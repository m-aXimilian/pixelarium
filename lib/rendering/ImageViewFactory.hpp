#pragma once

#include "PixelariumImageViewDefault.hpp"
#include "resources/resource.hpp"
namespace pixelarium::render
{
class ImageViewFactory
{
    using Image = imaging::IPixelariumImage;
    using Pool = resources::ImageResourcePool;

   public:
    explicit ImageViewFactory(Pool& pool) : image_pool_(pool) {}

    std::unique_ptr<PixelariumImageViewDefault> RenderImage(size_t id);

   private:
    Pool& image_pool_;
};
}  // namespace pixelarium::render
