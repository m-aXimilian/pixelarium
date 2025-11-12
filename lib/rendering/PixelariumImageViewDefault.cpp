#include "PixelariumImageViewDefault.hpp"

#include <format>

#include "RenderHelpers.hpp"
#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
/// @brief Displays the image in an ImGui window.
///
/// If the image is null, empty, or has an empty name, the function returns immediately.  Otherwise, it creates an ImGui
/// window with a horizontal scrollbar and menu bar. The image is rendered using the CvMatRender object, resizing it to
/// fit the available window space.  The raw and rendered dimensions are displayed below the image.
void pixelarium::render::PixelariumImageViewDefault::ShowImage()
{
    if (this->cached_image_.empty() || this->is_dirty_)
    {
        this->cached_image_ = this->img_->TryGetImage().value_or(cv::Mat{});
        this->is_dirty_ = false;
    }

    if (this->img_->Empty() || this->img_->type_ == imaging::ImageFileType::kUnknown || this->cached_image_.empty() ||
        this->img_->Name().empty())
    {
        // do nothing
        return;
    }

    ImGui::Begin(this->img_->Name().c_str(), &this->open_p,
                 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    this->curr_dim_ = ImGui::GetContentRegionAvail();
    auto new_dim = ImGui::GetContentRegionAvail();
    auto texture =
        dim_changed_p(this->curr_dim_, new_dim)
            ? this->render_.Render(static_cast<size_t>(this->curr_dim_.x), static_cast<size_t>(this->curr_dim_.y))
            : this->render_.Render();

    this->curr_dim_ = new_dim;

    ImVec2 dim(cached_image_.cols, cached_image_.rows);

    ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(texture)),
                 aspect_const_dimensions(dim, new_dim));

    ImGui::Separator();
    ImGui::Text("%s", std::format("   Raw Dimensions W : {}, H: {}", dim.x, dim.y).c_str());
    ImGui::Text("%s", std::format("Render Dimensions W : {}, H: {}", curr_dim_.x, curr_dim_.y).c_str());

    ImGui::End();
}
