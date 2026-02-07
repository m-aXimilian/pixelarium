#include "PixelariumImageViewCzi.hpp"

#include <format>
#include <memory>
#include <stdexcept>

#include "CvMatRender.hpp"
#include "IPixelariumImage.hpp"
#include "PixelariumCzi.hpp"
#include "RenderHelpers.hpp"
#include "imgui.h"

void pixelarium::application::PixelariumImageViewCzi::RefreshCachedImage()
{
    if (this->cached_image_.empty() || this->is_dirty_)
    {
        log_.Info(std::format("{}: refreshing image.", __PRETTY_FUNCTION__));
        imaging::CziParams params;
        params.dimension_map = this->dimension_map_;
        this->cached_image_ = this->img_->TryGetImage(params).value_or(cv::Mat{});
        // Resetting the image while the renderer is possibly accessing the
        // image at the same time is not a good idea. Therefore, we simply create
        // a new renderer here.
        this->render_ = std::make_unique<CvMatRender>(this->cached_image_);
        this->is_dirty_ = false;
    }
}

pixelarium::application::PixelariumImageViewCzi::PixelariumImageViewCzi(std::shared_ptr<Image> img, const Log& log)
    : log_(log)
{
    img_ = img;
    auto czi_img = std::static_pointer_cast<imaging::PixelariumCzi>(this->img_);

    auto stats = czi_img->GetStatistics();
    stats.dimBounds.EnumValidDimensions(
        [&](libCZI::DimensionIndex dim, int start, int) -> bool
        {
            this->dimension_map_[dim] = start;
            return true;
        });

    auto render_mat = img->TryGetImage();
    if (render_mat.has_value())
    {
        this->render_ = std::make_unique<CvMatRender>(render_mat.value());
    }
    else
    {
        auto msg{std::format("{}: fetching image failed: {}", __PRETTY_FUNCTION__, img->Name())};
        log_.Error(msg);
        throw std::runtime_error(msg);
    }

    // this->SetInitialSize();
    log_.Info(std::format("{}: dimension map size: {}", __PRETTY_FUNCTION__, dimension_map_.size()));
}

/// @brief Displays the image in an ImGui window.
///
/// If the image is null, empty, or has an empty name, the function returns immediately.  Otherwise, it creates an ImGui
/// window with a horizontal scrollbar and menu bar. The image is rendered using the CvMatRender object, resizing it to
/// fit the available window space.  The raw and rendered dimensions are displayed below the image.
void pixelarium::application::PixelariumImageViewCzi::ShowImage()
{
    auto czi_img = std::static_pointer_cast<imaging::PixelariumCzi>(this->img_);

    if (!czi_img) return;

    RefreshCachedImage();

    if (czi_img->Empty() || this->img_->type_ == imaging::ImageFileType::kUnknown || cached_image_.empty() ||
        czi_img->Name().empty())
    {
        // do nothing
        return;
    }

    if (first_render_)
    {
        first_render_ = false;
        constexpr auto initial_width{700.0f};
        const auto cached_width{cached_image_.cols};
        const auto cached_heigth{cached_image_.rows};
        const auto ratio{static_cast<float>(cached_heigth) / cached_width};
        SetInitialSize(initial_width, (initial_width * ratio + 100));
    }

    ImGui::Begin(this->img_->Name().c_str(), &this->open_p,
                 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    ImageViewMenuBar();

    this->curr_dim_ = ImGui::GetContentRegionAvail();
    auto new_dim = ImGui::GetContentRegionAvail();
    auto texture =
        dim_changed_p(this->curr_dim_, new_dim)
            ? this->render_->Render(static_cast<size_t>(this->curr_dim_.x), static_cast<size_t>(this->curr_dim_.y))
            : this->render_->Render();

    this->curr_dim_ = new_dim;

    ImVec2 dim(cached_image_.cols, cached_image_.rows);

    ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(texture)),
                 aspect_const_dimensions(dim, new_dim));

    ImGui::Separator();
    ImGui::Text("%s", std::format("   Raw Dimensions W : {}, H: {}", dim.x, dim.y).c_str());
    ImGui::Text("%s", std::format("Render Dimensions W : {}, H: {}", curr_dim_.x, curr_dim_.y).c_str());
    ImGui::Text("Dimensions");
    ImGui::Separator();
    if (ImGui::Button("Update"))
    {
        this->ForceUpdate();
    }

    auto stats = czi_img->GetStatistics();
    stats.dimBounds.EnumValidDimensions(
        [&](libCZI::DimensionIndex dim, int start, int size) -> bool
        {
            auto dim_char = libCZI::Utils::DimensionToChar(dim);
            if (ImGui::SliderInt(std::format("{}({}-{})", dim_char, start, size).c_str(), &dimension_map_[dim], start,
                                 size - 1))
            {
                this->ForceUpdate();
            }

            return true;
        });

    ImGui::End();
}
