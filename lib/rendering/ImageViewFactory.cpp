#include "ImageViewFactory.hpp"

#include <format>
#include <memory>

#include "imaging/PixelariumImageFactory.hpp"
#include "rendering/IPixelariumImageView.hpp"
#include "rendering/PixelariumImageViewCzi.hpp"
#include "rendering/PixelariumImageViewDefault.hpp"

/// @brief Creates a PixelariumImageView from a resource image.
/// @param image_id The ID of the image resource to render.
/// @return A unique pointer to the PixelariumImageView, or nullptr if the image resource is not found or is empty.  The
/// image data is copied.
std::unique_ptr<pixelarium::render::IPixelariumImageView> pixelarium::render::ImageViewFactory::RenderImage(
    resources::ResourceKey image_id)
{
    auto res{this->image_pool_.GetResource(image_id)};

    auto img{res.lock()};

    if (img == nullptr)
    {
        return {};
    }

    if (img->Empty())
    {
        return {};
    }

    auto type = imaging::ExtensionToType(img->Uri().extension().string());

    switch (type)
    {
        case imaging::ImageFileType::kUnknown:
        case imaging::ImageFileType::kAbstract:
            return {};
        case imaging::ImageFileType::kPng:
        case imaging::ImageFileType::kJpg:
            log_.Info(std::format("{}: Creating a Default View", __PRETTY_FUNCTION__));
            // beware: here we copy the actual image resource over to the new image
            return std::make_unique<PixelariumImageViewDefault>(img);
        case imaging::ImageFileType::kCzi:
            log_.Info(std::format("{}: Creating a CZI View", __PRETTY_FUNCTION__));
            // beware: here we copy the actual image resource over to the new image
            return std::make_unique<PixelariumImageViewCzi>(img, log_);
        default:
            return {};
    }
}
