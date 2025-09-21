#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>

namespace pixelarium::imaging
{
using AccessorFunctor = std::function<void(const std::string&, void*, int*)>;

enum class ImageFileType
{
    ABSRACT = 0,
    PNG = 1,
    JPG = 2,
    CZI = 3,
};

/// @brief This aims to be a generic image abstraction
/// meant for codec specific implementation.

/// Todo: the above implies that most of the below implementations don't make sense for this class (c.f. cv::Mat
/// generation et.al.)
class PixelariumImage
{
   public:
    // get back the defaults
    // this means, that there has to be and API option to set
    // a resource which should trigger some sort of action
    // after setting

    explicit PixelariumImage(const std::string& uri);

    PixelariumImage() = default;
    PixelariumImage(const PixelariumImage& other)
    {
        // be ware!!
        // we make a copy of the image data here!
        img_ = std::make_unique<cv::Mat>(*other.img_);
        uri_ = other.uri_;
    };
    PixelariumImage(PixelariumImage&& other) noexcept : img_(std::move(other.img_)) {}
    PixelariumImage& operator=(const PixelariumImage& other)
    {
        this->img_ = std::make_unique<cv::Mat>(*other.img_);
        this->uri_ = other.uri_;

        return *this;
    };

    PixelariumImage& operator=(PixelariumImage&& other) noexcept
    {
        if (this != &other)
        {
            img_ = std::move(other.img_);
            uri_ = other.uri_;
        }

        return *this;
    }
    // this should probably vanish as it makes no sense
    // for multidimensional images (more than one frame)
    // -> we need some sort of accessor functionality

    ~PixelariumImage() = default;

    const cv::Mat& GetImage() const { return *this->img_.get(); }

    const std::string Name() const noexcept
    {
        if (!this->uri_.empty())
        {
            return this->uri_.filename().string();
        }

        return {};
    }

    bool Empty() const noexcept { return this->img_->empty(); }

    static ImageFileType Type() { return PixelariumImage::type_; }

   protected:
    std::unique_ptr<cv::Mat> img_;

    std::filesystem::path uri_;

    static ImageFileType type_;
};

}  // namespace pixelarium::imaging
