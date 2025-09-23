#pragma once

#include "PixelariumImageViewDefault.hpp"
#include "rendering/IPixelariumImageView.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
namespace pixelarium::render
{
class ImageViewFactory
{
    using Image = imaging::IPixelariumImage;
    using Pool = resources::ImageResourcePool;
    using Log = utils::log::ILog;

   public:
    explicit ImageViewFactory(Pool& pool, const Log& log) : image_pool_(pool), log_(log) {}

    std::unique_ptr<IPixelariumImageView> RenderImage(size_t id);

   private:
    Pool& image_pool_;
    const Log& log_;
};
}  // namespace pixelarium::render
