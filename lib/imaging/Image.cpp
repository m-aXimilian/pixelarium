#include "Image.hpp"

#include <filesystem>
#include <format>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>
#include <string_view>

pixelarium::imaging::Image::Image(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error(std::format("File not {} found", uri));
    }

    this->_img = std::make_unique<cv::Mat>(cv::imread(uri));
}