#include "PixelariumImageView.hpp"

#include <format>

#include "imgui.h"

/// @brief Checks if the dimensions of two ImVec2 vectors have changed significantly.
/// @param ref_rect The reference ImVec2 vector.
/// @param new_rect The new ImVec2 vector to compare against.
/// @return True if the absolute difference between the y-coordinates is greater than 5 or the x-coordinates are
/// different; otherwise, false.
static bool dim_changed_p(const ImVec2& ref_rect, const ImVec2& new_rect)
{
    if (std::abs(ref_rect.y - new_rect.y) > 5 || std::abs(ref_rect.x - new_rect.x))
    {
        return true;
    }

    return false;
}

/// @brief Calculates dimensions to maintain aspect ratio.
/// @param img The input image.
/// @param curr_dim The current dimensions.
/// @return The calculated dimensions maintaining aspect ratio.
ImVec2 aspect_const_dimensions(const pixelarium::imaging::PixelariumImage& img, const ImVec2& curr_dim)
{
    const auto w_fact = (static_cast<float>(curr_dim.x) / img.GetImage().cols);
    const auto h_fact = (static_cast<float>(curr_dim.y) / img.GetImage().rows);

    const auto fact = w_fact < h_fact ? w_fact : h_fact;

    return ImVec2(img.GetImage().cols * fact, img.GetImage().rows * fact);
}

/// @brief Displays the image in an ImGui window.
///
/// If the image is null, empty, or has an empty name, the function returns immediately.  Otherwise, it creates an ImGui
/// window with a horizontal scrollbar and menu bar. The image is rendered using the CvMatRender object, resizing it to
/// fit the available window space.  The raw and rendered dimensions are displayed below the image.
void pixelarium::render::PixelariumImageView::ShowImage()
{
    if (this->img_.Empty() || this->img_.Name().empty())
    {
        // do nothing
        return;
    }

    ImGui::Begin(img_.Name().c_str(), &this->open_p, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    this->curr_dim_ = ImGui::GetContentRegionAvail();
    auto new_dim = ImGui::GetContentRegionAvail();
    auto texture =
        dim_changed_p(this->curr_dim_, new_dim)
            ? this->render_.Render(static_cast<size_t>(this->curr_dim_.x), static_cast<size_t>(this->curr_dim_.y))
            : this->render_.Render();

    this->curr_dim_ = new_dim;

    ImVec2 dim(this->img_.GetImage().cols, this->img_.GetImage().rows);

    ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(texture)),
                 aspect_const_dimensions(this->img_, new_dim));

    ImGui::Separator();
    ImGui::Text("%s", std::format("   Raw Dimensions W : {}, H: {}", dim.x, dim.y).c_str());
    ImGui::Text("%s", std::format("Render Dimensions W : {}, H: {}", curr_dim_.x, curr_dim_.y).c_str());

    ImGui::End();
}
