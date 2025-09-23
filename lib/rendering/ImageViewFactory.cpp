#include "ImageViewFactory.hpp"

#include <memory>
#include "imaging/PixelariumImageFactory.hpp"
#include "rendering/IPixelariumImageView.hpp"
#include "rendering/PixelariumImageViewDefault.hpp"

/// @brief Creates a PixelariumImageView from a resource image.
/// @param image_id The ID of the image resource to render.
/// @return A unique pointer to the PixelariumImageView, or nullptr if the image resource is not found or is empty.  The image data is copied.
std::unique_ptr<pixelarium::render::IPixelariumImageView> pixelarium::render::ImageViewFactory::RenderImage(
    size_t image_id)
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
        case imaging::ImageFileType::UNKNOWN:
        case imaging::ImageFileType::ABSTRACT:
            return {};
        case imaging::ImageFileType::PNG:
        case imaging::ImageFileType::JPG:
            log_.Info("Creating a Default View");
            // beware: here we copy the actual image resource over to the new image
            return std::make_unique<PixelariumImageViewDefault>(img);
        case imaging::ImageFileType::CZI:
            log_.Info("{}: Creating a CZI View");
            // beware: here we copy the actual image resource over to the new image
            return std::make_unique<PixelariumImageViewDefault>(img);
            // return std::make_unique<PixelariumImageViewCzi>(img);
        default:
            return {};
    }
}

