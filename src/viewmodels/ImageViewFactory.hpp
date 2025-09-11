#pragma once

#include "PixelariumImage.hpp"
#include "resources/resource.hpp"
#include "views/PixelariumImageView.hpp"
namespace pixelarium::ui
{
class ImageViewFactory
{
    using Image = imaging::PixelariumImage;
    using Pool = resources::ImageResourcePool;

   public:
    explicit ImageViewFactory(Pool& pool) : image_pool_(pool) {}

    std::unique_ptr<PixelariumImageView> RenderImage(size_t id);

   private:
    Pool& image_pool_;
};
}  // namespace pixelarium::ui
