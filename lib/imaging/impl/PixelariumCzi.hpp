#pragma once

#include <memory>
#include <string>

#include "../IPixelariumImage.hpp"
#include "libCZI.h"

namespace pixelarium::imaging
{
struct CziParams : public IImageQuery
{
    
};

class PixelariumCzi : public IPixelariumImage
{
   public:
    explicit PixelariumCzi(const std::string& uri);
    ~PixelariumCzi()
    {
        if (this->czi_reader_)
            this->czi_reader_->Close();
    }

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

    const libCZI::SubBlockStatistics& GetStatistics() const { return this->image_statistics_; }

   public:
    const static ImageFileType type_{ImageFileType::CZI};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};

    libCZI::SubBlockStatistics image_statistics_;

    std::shared_ptr<libCZI::ICZIReader> czi_reader_;
};
}  // namespace pixelarium::imaging
