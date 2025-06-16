#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>

namespace pixelarium::imaging
{
class PixelariumImage
{
   public:

    explicit PixelariumImage(const std::string& uri);

    // get back the defaults
    PixelariumImage() = default;
    PixelariumImage(const PixelariumImage& other)
    {
        // be ware!!
        // we make a copy of the image data here!
        img_ = std::make_unique<cv::Mat>(*other.img_);
    };
    PixelariumImage(PixelariumImage&& other) noexcept
        : img_(std::move(other.img_)) {}
    // requires a copy ctor which we don't have
    PixelariumImage& operator=(const PixelariumImage& other) = delete;
    PixelariumImage& operator=(PixelariumImage&& other) noexcept
    {
        if (this != &other)
        {
            img_ = std::move(other.img_);
        }

        return *this;
    }

    ~PixelariumImage() = default;

    const cv::Mat& GetImage() const { return *this->img_.get(); }

   protected:
    std::unique_ptr<cv::Mat> img_;
};

}  // namespace pixelarium::imaging
