#pragma once

#include <memory>

#include "PixelariumImage.hpp"
#include "imgui.h"
#include "rendering/CvMatRender.hpp"

namespace pixelarium::ui
{
class PixelariumImageView
{
    using Image = imaging::PixelariumImage;
    using Render = render::CvMatRender;

   public:
    explicit PixelariumImageView(const std::shared_ptr<Image>& img) : img_(img) { render_ = Render(img_); }
    PixelariumImageView() = delete;
    PixelariumImageView(PixelariumImageView&) = delete;
    PixelariumImageView(const PixelariumImageView&) = delete;
    PixelariumImageView(PixelariumImageView&&) = delete;
    PixelariumImageView& operator=(PixelariumImageView&) = delete;
    PixelariumImageView& operator=(PixelariumImageView&&) = delete;

    void ToggleView(bool target) { open_p = target; }
    void ShowImage();

   private:
    const std::shared_ptr<Image> img_;
    Render render_;
    bool open_p{false};
    ImVec2 curr_dim_{};
};
}  // namespace pixelarium::ui
