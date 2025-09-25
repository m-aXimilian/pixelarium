#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>

namespace pixelarium::imaging
{
using ImageQueryFunctor = std::function<void(const std::string&, void*, int*)>;

/// @brief Enumeration of supported image file types.
enum class ImageFileType
{
    /// @brief Represents an unknown or unsupported file type.
    UNKNOWN = -10,
    /// @brief Represents an abstract image type (e.g., a placeholder).
    ABSTRACT = 0,
    /// @brief Represents a PNG image file.
    PNG = 1,
    /// @brief Represents a JPG image file.
    JPG = 2,
    /// @brief Represents a CZI image file.
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

    /// @brief Attempts to retrieve the image.
    /// @return A unique pointer to a Mat object containing the image data,
    /// or nullptr if the image is not found or cannot be retrieved.
    /// May throw exceptions for multidimensional images.
    virtual std::unique_ptr<cv::Mat> TryGetImage() = 0;

    /// @brief Attempts to retrieve the image.
    /// @return A unique pointer to a Mat object containing the image data,
    /// or nullptr if the image is not found or cannot be retrieved.
    virtual std::unique_ptr<cv::Mat> TryGetImage(const IImageQuery&) = 0;

    /// @brief Attempts to retrieve a collection of images based on a query.
    /// @param query The query object defining the images to retrieve.
    /// @return A vector of unique pointers to cv::Mat objects. Each element is an image.
    /// Returns an empty vector if no images are found or if an error occurs.
    virtual std::vector<std::unique_ptr<cv::Mat>> TryGetImages(const IImageQuery&) = 0;

    /// @brief Checks if the image is empty.
    /// @return true if the image is empty, false otherwise.
    virtual bool Empty() const noexcept = 0;

    // default implemented
   public:
    /// @brief  Gets the resource identifier as a file path.
    /// @return @c std::filesystem::path of the underlying resource.
    virtual std::filesystem::path Uri() const noexcept { return this->uri_; }

    /// @brief  Gets the resource name.
    /// @note   Implementations of IPixelariumImage that live in memory
    ///         should override this to get something meaningful as the name
    ///         cannot be fetched from the resource uri in that case.
    /// @return The name of the underlying resource.
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
