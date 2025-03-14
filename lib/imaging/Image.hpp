#pragma once

#include <filesystem>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

namespace pixelarium::imaging
{
class Image
{
   public:

    explicit Image(const std::string& uri);

    // get back the defaults
    Image() = default;
    Image(const Image& other) = default;
    Image(Image&& other) noexcept = default;
    Image& operator=(const Image& other) = default;
    Image& operator=(Image&& other) noexcept = default;
    ~Image() = default;

    const cv::Mat& GetImage() const { return this->_img; }

   private:
    cv::Mat _img;
};

}  // namespace pixelarium::imaging