#pragma once

#include <stdexcept>
#include <string>

#include "../IPixelariumImage.hpp"

namespace pixelarium::imaging
{
/// @brief Implements support for .png-images in the realm of IPixelariumImage
class PixelariumPng : public IPixelariumImage
{
   public:
    explicit PixelariumPng(const std::string& url);

    // IPixelariumImage member implementations
   public:
    std::optional<cv::Mat> TryGetImage() override;

    std::optional<cv::Mat> TryGetImage(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with png.");
    }

    std::vector<std::optional<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with png.");
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::kPng};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
};
}  // namespace pixelarium::imaging
