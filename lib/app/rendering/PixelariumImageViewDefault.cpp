#include "PixelariumImageViewDefault.hpp"

#include <cstdio>
#include <format>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <print>
#include <ranges>

#include "RenderHelpers.hpp"
#include "app_resources_default.h"
#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
#include "implot.h"
#include "utilities/simple_thread_pool.hpp"

void pixelarium::application::PixelariumImageViewDefault::RefreshCachedImage()
{
    if (this->cached_image_.empty() || this->is_dirty_)
    {
        this->cached_image_ = this->img_->TryGetImage().value_or(cv::Mat{});
        this->is_dirty_ = false;
    }
}

void pixelarium::application::PixelariumImageViewDefault::ImageViewMenuBarAdditions()
{
    ImGui::MenuItem("Histogram", NULL, &this->show_hists_);
}

/// @brief Displays the image in an ImGui window.
///
/// If the image is null, empty, or has an empty name, the function returns immediately.  Otherwise, it creates an ImGui
/// window with a horizontal scrollbar and menu bar. The image is rendered using the CvMatRender object, resizing it to
/// fit the available window space.  The raw and rendered dimensions are displayed below the image.
void pixelarium::application::PixelariumImageViewDefault::ShowImage()
{
    RefreshCachedImage();

    if (this->img_->Empty() || this->img_->type_ == imaging::ImageFileType::kUnknown || this->cached_image_.empty() ||
        this->img_->Name().empty())
    {
        // do nothing
        return;
    }

    if (first_render_)
    {
        first_render_ = false;

        const auto cached_width{cached_image_.cols};
        const auto cached_heigth{cached_image_.rows};
        const auto ratio{static_cast<float>(cached_heigth) / cached_width};
        SetInitialSize(kInitialWindowWidth, (kInitialWindowWidth * ratio + 100));
        utils::simple_thread_pool::run_asynch([this]() { GenerateHistogram(); });
    }

    ImGui::Begin(this->img_->Name().c_str(), &this->open_p,
                 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    ImageViewMenuBar();

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

    if (show_hists_ && hist_available_)
    {
        if (ImPlot::BeginPlot("Histogram"))
        {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            std::string name{"ab"};
            for (auto& e : hist_planes_)
            {
                ImPlot::PlotHistogram(name.c_str(), e.data, e.rows * e.cols, 16);
                for (auto& e : name)
                {
                    e++;
                }
            }
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO,
                                       ImPlot::GetColormapColor(1));

            ImPlot::EndPlot();
        }
    }

    ImGui::Text("%s", std::format("   Raw Dimensions W : {}, H: {}", dim.x, dim.y).c_str());
    ImGui::Text("%s", std::format("Render Dimensions W : {}, H: {}", curr_dim_.x, curr_dim_.y).c_str());

    ImGui::End();
}

auto pixelarium::application::PixelariumImageViewDefault::GenerateHistogram() -> void
{
    cv::split(cached_image_, bgr_planes_);
    hist_planes_.resize(bgr_planes_.size());

    int histSize = 256;
    float range[] = {0, 256};  // the upper boundary is exclusive
    const float* histRange[] = {range};

    for (auto [bgr, hist] : std::ranges::views::zip(bgr_planes_, hist_planes_))
    {
        cv::calcHist(&bgr, 1, 0, cv::Mat(), hist, 1, &histSize, histRange, true, false);
    }

    std::print("Planes: {}, Hists: {}", bgr_planes_.size(), hist_planes_.size());

    hist_available_ = true;
}
