#include "ImageViewFactory.hpp"
#include <memory>
#include <optional>

using namespace pixelarium::ui;

std::unique_ptr<PixelariumImageView> ImageViewFactory::RenderImage(size_t image_id)
{
    auto img{this->image_pool_.GetResource(image_id)};

    if (!img.has_value() ||
        img.value()->Empty())
    {
        return nullptr;
    }

    // beware: here we copy the actual image resource over to the new image
    return std::make_unique<PixelariumImageView>(std::make_shared<Image>(*img.value()));
}
