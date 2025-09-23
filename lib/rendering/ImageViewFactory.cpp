#include "ImageViewFactory.hpp"

#include <memory>

/// @brief Creates a PixelariumImageView from a resource image.
/// @param image_id The ID of the image resource to render.
/// @return A unique pointer to the PixelariumImageView, or nullptr if the image resource is not found or is empty.  The image data is copied.
std::unique_ptr<pixelarium::render::PixelariumImageViewDefault> pixelarium::render::ImageViewFactory::RenderImage(
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

    // beware: here we copy the actual image resource over to the new image
    return std::make_unique<PixelariumImageViewDefault>(img);
}

