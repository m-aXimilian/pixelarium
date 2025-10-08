#pragma once

#include <memory>
#include <string>

#include "../IPixelariumImage.hpp"
#include "libCZI.h"
#include "utilities/ILog.hpp"

namespace pixelarium::imaging
{
/// @brief An implementation of IImageQuery to work on CZI images.
/// @note  Check the documentation here https://zeiss.github.io/libczi/pages/mainpage.html#czi-in-a-nutshell
struct CziParams : public IImageQuery
{
    /// @brief A map providing the start coordinate for each dimension in the CZI
    std::unordered_map<libCZI::DimensionIndex, int> dimension_map;
};

/// @brief Implements support for .czi-images in the realm of IPixelariumImage
class PixelariumCzi : public IPixelariumImage
{
    using Log = pixelarium::utils::log::ILog;

   public:
    explicit PixelariumCzi(const std::string& uri, const Log& log);
    ~PixelariumCzi()
    {
        if (this->czi_reader_) this->czi_reader_->Close();
    }

    // IPixelariumImage member implementations
   public:
    std::unique_ptr<cv::Mat> TryGetImage() override;

    std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) override;

    std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not implemented.");
    }

    bool Empty() const noexcept override { return this->is_empty_; }

    const libCZI::SubBlockStatistics& GetStatistics() const { return this->image_statistics_; }

   public:
    const static ImageFileType type_{ImageFileType::kCzi};

   private:
    std::unique_ptr<cv::Mat> SubblockToCvMat(int index);

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};

    libCZI::SubBlockStatistics image_statistics_;

    std::shared_ptr<libCZI::ICZIReader> czi_reader_;

    std::unordered_map<libCZI::DimensionIndex, int> dimension_map_;

    const Log& log_;
};
}  // namespace pixelarium::imaging
