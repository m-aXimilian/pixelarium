#pragma once

#include "IPixelariumImageView.hpp"
#include "resource.hpp"
#include "ILog.hpp"

namespace pixelarium::application
{
/// @brief Factory for instantiating matching views to different implementations of IPixelariumImage.
class ImageViewFactory
{
    using Image = imaging::IPixelariumImageCvMat;
    using Pool = resources::ImageResourcePool;
    using Log = utils::log::ILog;

   public:
    explicit ImageViewFactory(Pool& pool, const Log& log) : image_pool_(pool), log_(log) {}

    std::unique_ptr<IPixelariumImageView> RenderImage(resources::ResourceKey id);

   private:
    Pool& image_pool_;
    const Log& log_;
};
}  // namespace pixelarium::application
