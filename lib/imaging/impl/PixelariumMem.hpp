#pragma once

#include <stdexcept>
#include <string>

#include "../IPixelariumImage.hpp"
#include "utilities/ILog.hpp"

namespace pixelarium::imaging
{
/// @brief Implements support for in-images in the realm of IPixelariumImage
class PixelariumMem : public IPixelariumImage
{
    using Log = pixelarium::utils::log::ILog;
   public:
    explicit PixelariumMem(const cv::Mat& img, const std::string& name, const Log& log);

    // IPixelariumImage member implementations
   public:
    std::unique_ptr<cv::Mat> TryGetImage() override;

    std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) override
    {
        throw std::runtime_error("Not implemented.");
    }

    std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        throw std::runtime_error("Not implemented.");
    }

    void SetImage(const cv::Mat& img)
    {
        this->img_ = img;
    }

    std::string Name() const noexcept override
    {
        return this->name_;
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::kMemory};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
    cv::Mat img_;
    const Log& log_;
    std::string name_;
};
}  // namespace pixelarium::imaging
