#pragma once

#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"
#include "rendering/CvMatRender.hpp"

namespace pixelarium::render
{
class PixelariumImageView
{
    using Image = imaging::IPixelariumImage;
    using Render = render::CvMatRender;

   public:
    explicit PixelariumImageView(std::shared_ptr<Image> img) : img_(img) { render_ = Render(img_); }
    PixelariumImageView() = delete;
    PixelariumImageView(PixelariumImageView&) = delete;
    PixelariumImageView(const PixelariumImageView&) = delete;
    PixelariumImageView(PixelariumImageView&&) = delete;
    PixelariumImageView& operator=(PixelariumImageView&) = delete;
    PixelariumImageView& operator=(PixelariumImageView&&) = delete;

    // void ToggleView(bool target) { open_p = target; }
    const bool* GetStatus() const noexcept { return &this->open_p; }
    void ShowImage();

   private:
    std::shared_ptr<Image> img_;
    std::optional<std::unique_ptr<cv::Mat>> cached_image_ {};
    Render render_;
    bool open_p{true};
    ImVec2 curr_dim_{};
};
}  // namespace pixelarium::render
