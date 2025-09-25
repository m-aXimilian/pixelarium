#include "RenderHelpers.hpp"

/// @brief Checks if the dimensions of two ImVec2 vectors have changed significantly.
/// @param ref_rect The reference ImVec2 vector.
/// @param new_rect The new ImVec2 vector to compare against.
/// @return True if the absolute difference between the y-coordinates is greater than 5 or the x-coordinates are
/// different; otherwise, false.
bool pixelarium::render::dim_changed_p(const ImVec2& ref_rect, const ImVec2& new_rect)
{
    if (abs(ref_rect.y - new_rect.y) > 5 || abs(ref_rect.x - new_rect.x))
    {
        return true;
    }

    return false;
}

/// @brief Calculates dimensions to maintain aspect ratio.
/// @param img The input image.
/// @param curr_dim The current dimensions.
/// @return The calculated dimensions maintaining aspect ratio.
ImVec2 pixelarium::render::aspect_const_dimensions(const ImVec2& raw_dim, const ImVec2& curr_dim)
{
    const auto w_fact = (curr_dim.x / raw_dim.x);
    const auto h_fact = (curr_dim.y / raw_dim.y);

    const auto fact = w_fact < h_fact ? w_fact : h_fact;

    return ImVec2(raw_dim.x * fact, raw_dim.y * fact);
}
