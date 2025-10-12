#include "PixelariumMem.hpp"

#include <filesystem>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <string>

pixelarium::imaging::PixelariumMem::PixelariumMem(const cv::Mat& img, const std::string& name, const Log& log)
    : img_(img), log_(log), name_(name)
{
    this->is_empty_ = false;
    this->uri_ = std::filesystem::path();
}

std::unique_ptr<cv::Mat> pixelarium::imaging::PixelariumMem::TryGetImage()
{
    // ToDo: this craving for a revision of the whole concept:
    // the interface requires a unique_ptr here. This concept was designed to "create an in-memory image on demand" sort
    // of.
    // I.e., it only makes sense for the file types that do not already manage a cv::Mat in memory.
    // PixelariumMem is meant for exactly this in-memory management of a cv::Mat though.
    // So, returning a unique_ptr from it in the following semantic essentially calls the
    // copy constructor of cv::Mat. This is potentially not "super bad", but at least it requires attention at some
    // point.
    return std::make_unique<cv::Mat>(this->img_);
}
