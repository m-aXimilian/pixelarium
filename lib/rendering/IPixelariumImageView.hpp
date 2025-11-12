#pragma once

#include <memory>

#include "imaging/IPixelariumImage.hpp"
#include "imgui.h"

namespace pixelarium::render
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
    virtual void SetInitialSize(float width = 700.0f, float height = 700.0f)
    {
        ImGui::SetNextWindowSize({width, height});
    }

   protected:
    std::shared_ptr<imaging::IPixelariumImage> img_{};
    cv::Mat cached_image_{};
    bool open_p{true};
    bool is_dirty_{true};
};
}  // namespace pixelarium::render
