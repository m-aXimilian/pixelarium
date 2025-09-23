#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>

namespace pixelarium::imaging
{
using ImageQueryFunctor = std::function<void(const std::string&, void*, int*)>;

enum class ImageFileType
{
    UNKNOWN = -10,
    ABSTRACT = 0,
    PNG = 1,
    JPG = 2,
    CZI = 3,
};

/// @brief An abstract interface to define a semantic query
/// for a multi-dimensional image.
struct IImageQuery
{
    virtual ~IImageQuery() = default;
};

/// @brief This aims to be a generic image abstraction
/// meant for codec specific implementation.
class IPixelariumImage
{
   public:
    virtual ~IPixelariumImage() = default;

    // this will have to throw or something for multidimensional images
    virtual std::unique_ptr<cv::Mat> TryGetImage() = 0;

    virtual std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) = 0;

    virtual std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) = 0;

    virtual bool Empty() const noexcept = 0;

    // default implemented
   public:
    virtual std::filesystem::path Uri() const noexcept { return this->uri_; }

    virtual std::string Name() const noexcept
    {
        if (!this->uri_.empty())
        {
            return this->uri_.filename().string();
        }

        return {};
    }

   public:
    const static ImageFileType type_{ImageFileType::ABSTRACT};

   protected:
    std::filesystem::path uri_;
};

}  // namespace pixelarium::imaging
