#pragma once

#include <memory>
#include <string>

#include "ILog.hpp"
#include "IPixelariumImage.hpp"
#include "libCZI.h"
#include "libCZI_Pixels.h"

namespace
{
struct CvPixelTypeAndSize
{
    int size{-1};
    int type{-1};
};

template <typename P>
[[nodiscard]] constexpr auto GetCVPixelTypeAndSize(P p) noexcept -> CvPixelTypeAndSize
{
    switch (p)
    {
        case libCZI::PixelType::Gray8:
            return {1, CV_8U};
        case libCZI::PixelType::Gray16:
            return {2, CV_16U};
        case libCZI::PixelType::Bgr24:
            return {3, CV_8UC3};
        case libCZI::PixelType::Bgra32:
            return {4, CV_8UC4};
        case libCZI::PixelType::Gray32:
            return {4, CV_32S};
        case libCZI::PixelType::Gray32Float:
            return {4, CV_32F};
        default:
            return {};
    }
}
}  // namespace

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
class PixelariumCzi : public IPixelariumImageCvMat
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
    std::optional<cv::Mat> TryGetImage() override;

    std::optional<cv::Mat> TryGetImage(const IImageQuery&) override;

    std::vector<std::optional<cv::Mat>> TryGetImages(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not implemented.");
    }

   public:
    bool Empty() const noexcept override { return this->is_empty_; }

    const libCZI::SubBlockStatistics& GetStatistics() const { return this->image_statistics_; }

   public:
    const static ImageFileType type_{ImageFileType::kCzi};

   private:
    std::optional<cv::Mat> SubblockToCvMat(int index);

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
