#include "Image.hpp"

#include <filesystem>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>

pixelarium::imaging::Image::Image(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error("File not found");
    }

    this->_img = cv::imread(uri);
}