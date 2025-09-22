#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <optional>
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
    virtual std::optional<std::unique_ptr<cv::Mat>> TryGetImage() = 0;

    virtual std::optional<std::unique_ptr<cv::Mat>> TryGetImage(const IImageQuery&) = 0;

    virtual std::string Name() const noexcept = 0;

    virtual bool Empty() const noexcept = 0;

    virtual std::filesystem::path Uri() const noexcept = 0;

   public:
    const static ImageFileType type_{ImageFileType::ABSTRACT};

   protected:
    std::filesystem::path uri_;
};

}  // namespace pixelarium::imaging
