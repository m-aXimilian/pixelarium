#include "PixelariumTiff.hpp"

#include <filesystem>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <string>

pixelarium::imaging::PixelariumTiff::PixelariumTiff(const std::string& uri, const Log& log)  : log_(log)
{
    if (!std::filesystem::exists(uri))
    {
        throw std::runtime_error("Render file not found.");
    }

    this->is_empty_ = false;
    this->uri_ = std::filesystem::path(uri);
}

std::unique_ptr<cv::Mat> pixelarium::imaging::PixelariumTiff::TryGetImage()
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
