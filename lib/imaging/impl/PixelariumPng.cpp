#include "PixelariumPng.hpp"

#include <filesystem>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <string>

pixelarium::imaging::PixelariumPng::PixelariumPng(const std::string& uri)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error(std::format("File not {} found", uri));
    }

    this->uri_ = std::filesystem::path(uri);
}

std::optional<std::unique_ptr<cv::Mat>> pixelarium::imaging::PixelariumPng::TryGetImage()
{
    try
    {
        auto img = std::make_unique<cv::Mat>(cv::imread(this->uri_.string()));

        this->is_empty_ = false;

        return img;
    }
    catch (const std::exception& e)
    {
        this->is_empty_ = true;
        return {};
    }
}
