#include "ThumbnailFactory.hpp"

namespace pixelarium::imaging
{

template <typename R, typename D>
auto ThumbnailFactory<R, D>::GetThumbnail(resources::ResourceKey key) -> const R&
{
    if (const auto& thumb = inner_pool_.GetResource(key).lock(); thumb)
    {
        return thumb;
    }

    if (const auto& res = pool_.GetResource(key).lock(); res)
    {
        
    }
}
}  // namespace pixelarium::imaging
