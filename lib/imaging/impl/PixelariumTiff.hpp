#pragma once

#include <stdexcept>
#include <string>

#include "../IPixelariumImage.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::imaging
{
/// @brief Implements support for .tiff-images in the realm of IPixelariumImage
class PixelariumTiff : public IPixelariumImage
{
    using Log = pixelarium::utils::log::ILog;
   public:
    explicit PixelariumTiff(const std::string& uri, const Log& log);

    // IPixelariumImage member implementations
   public:
    std::unique_ptr<cv::Mat> TryGetImage() override;

    std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with tiff.");
    }

    std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with tiff.");
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::kTiff};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
    const Log& log_;
};
}  // namespace pixelarium::imaging
