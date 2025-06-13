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
    // we cannot copy an Image since this conflicts with the _img field
    PixelariumImage(const PixelariumImage& other) = delete;
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
