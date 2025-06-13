#include "PixelariumImage.hpp"

#include <filesystem>
#include <format>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>

pixelarium::imaging::PixelariumImage::PixelariumImage(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error(std::format("File not {} found", uri));
    }

    this->img_ = std::make_unique<cv::Mat>(cv::imread(uri));
}
