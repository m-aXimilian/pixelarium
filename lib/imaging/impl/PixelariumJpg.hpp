#pragma once

#include <stdexcept>
#include <string>

#include "../IPixelariumImage.hpp"

namespace pixelarium::imaging
{
/// @brief Implements support for .jpg-images in the realm of IPixelariumImage
class PixelariumJpg : public IPixelariumImageCvMat
{
   public:
    explicit PixelariumJpg(const std::string& url);

    // IPixelariumImage member implementations
   public:
    std::optional<cv::Mat> TryGetImage() override;

    std::optional<cv::Mat> TryGetImage(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with jpg.");
    }

    std::vector<std::optional<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with jpg.");
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::kJpg};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
};
}  // namespace pixelarium::imaging
