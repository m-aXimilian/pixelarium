#pragma once

#include "imgui.h"

namespace pixelarium::render
{
bool dim_changed_p(const ImVec2& ref_rect, const ImVec2& new_rect);

ImVec2 aspect_const_dimensions(const ImVec2& raw_dim, const ImVec2& curr_dim);
};  // namespace pixelarium::render
