#pragma once

#include "resource.hpp"

namespace pixelarium::imaging
{
namespace res = resources;
template <typename R, typename D>
class ThumbnailFactory
{
   public:
    explicit ThumbnailFactory(const res::IResourcePool<R, D>& pool) : pool_(pool) {}
    ThumbnailFactory() = delete;

    auto GetThumbnail(resources::ResourceKey) -> const R&;

   private:
    const res::IResourcePool<R, D>& pool_;
    res::IResourcePool<R, D> thumb_pool_;
};
}  // namespace pixelarium::imaging
