#pragma once

#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "rendering/CvMatRender.hpp"

namespace pixelarium::render
{
class IPixelariumImageView
{
   public:
    virtual ~IPixelariumImageView() = default;
    virtual void ShowImage() = 0;

    // default implemented
   public:
    virtual const bool* GetStatus() const noexcept { return &this->open_p; }
    virtual void ForceUpdate() noexcept { this->is_dirty_ = true; }

   protected:
    std::shared_ptr<imaging::IPixelariumImage> img_{};
    std::unique_ptr<cv::Mat> cached_image_{};
    render::CvMatRender render_;
    bool open_p{true};
    bool is_dirty_{true};
};
}  // namespace pixelarium::render
