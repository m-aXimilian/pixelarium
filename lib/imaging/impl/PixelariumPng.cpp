#include "PixelariumPng.hpp"

#include <filesystem>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <string>

pixelarium::imaging::PixelariumPng::PixelariumPng(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error("Render file not found.");
    }

    this->is_empty_ = false;
    this->uri_ = std::filesystem::path(uri);
}

std::optional<cv::Mat> pixelarium::imaging::PixelariumPng::TryGetImage()
{
    try
    {
        auto img = cv::Mat(cv::imread(this->uri_.string()));

        this->is_empty_ = false;

        return img;
    }
    catch (const std::exception& e)
    {
        this->is_empty_ = true;
        return {};
    }
}
