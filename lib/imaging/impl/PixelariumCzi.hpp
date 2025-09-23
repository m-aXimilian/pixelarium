#pragma once

#include <string>

#include "../IPixelariumImage.hpp"

namespace pixelarium::imaging
{
struct CziParams : public IImageQuery
{
    
};

class PixelariumCzi : public IPixelariumImage
{
   public:
    explicit PixelariumCzi(const std::string& uri);

    // IPixelariumImage member implementations
   public:
    std::unique_ptr<cv::Mat> TryGetImage() override;

    std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not implemented.");
    }

    std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not implemented.");
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::CZI};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
};
}  // namespace pixelarium::imaging
