#pragma once

#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>

namespace pixelarium::imaging
{
class Image
{
   public:

    explicit Image(const std::string& uri);

    // get back the defaults
    Image() = default;
    // we cannot copy an Image since this conflicts with the _img field
    Image(const Image& other) = delete;
    Image(Image&& other) noexcept = default;
    // requires a copy ctor which we don't have
    Image& operator=(const Image& other) = delete;
    Image& operator=(Image&& other) noexcept = default;
    ~Image() = default;

    const cv::Mat& GetImage() const { return *this->_img.get(); }

   private:
    std::unique_ptr<cv::Mat> _img;
};

}  // namespace pixelarium::imaging