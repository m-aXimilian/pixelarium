#pragma once

#include <memory>

#include "IPixelariumImage.hpp"
#include "imgui.h"

namespace pixelarium::application
{
/// @brief An interface defining the contract on views to dedicated implementations of IPixelariumImage
class IPixelariumImageView
{
   public:
    virtual ~IPixelariumImageView() = default;
    virtual void ShowImage() = 0;

    // default implemented
   public:
    virtual const bool* GetStatus() const noexcept { return &this->open_p; }
    virtual void ForceUpdate() noexcept { this->is_dirty_ = true; }

    // this must be called immediately before a "ImGui::Begin" context
    // as it will affect the next window and result in undeterministic effects
    // when called "out of sync"
    virtual void SetInitialSize(float width = 700.0f, float height = 700.0f)
    {
        ImGui::SetNextWindowSize({width, height});
    }

   protected:
    virtual void ImageViewMenuBar();
    virtual void ImageViewMenuBarAdditions() {};

   protected:
    std::shared_ptr<imaging::IPixelariumImageCvMat> img_{};
    cv::Mat cached_image_{};
    bool open_p{true};
    bool is_dirty_{true};
    bool first_render_{true};
};
}  // namespace pixelarium::application
