#pragma once

#include <stdexcept>
#include <string>

#include "../IPixelariumImage.hpp"

namespace pixelarium::imaging
{
class PixelariumPng : public IPixelariumImage
{
   public:
    explicit PixelariumPng(const std::string& url);

    // IPixelariumImage member implementations
   public:
    std::optional<std::unique_ptr<cv::Mat>> TryGetImage() override;

    std::optional<std::unique_ptr<cv::Mat>> TryGetImage(const IImageQuery&) override
    {
        // ToDo: proper error
        throw std::runtime_error("Not possible with png.");
    }

    std::string Name() const noexcept override
    {
        if (!this->uri_.empty())
        {
            return this->uri_.filename().string();
        }

        return {};
    }

    std::filesystem::path Uri() const noexcept override
    {
        return this->uri_.string();
    }

    bool Empty() const noexcept override { return this->is_empty_; }

   public:
    const static ImageFileType type_{ImageFileType::PNG};

   private:
    // this should be set by each image getter
    // after a new cv::Mat could be instantiated
    bool is_empty_{true};
};
}  // namespace pixelarium::imaging
