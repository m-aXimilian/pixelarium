#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "viewmodels/ImageViewFactory.hpp"
#include "views/PixelariumImageView.hpp"

// This is intended as an additional abstraction
// aggregating views that should be rendered (or not)
namespace pixelarium::ui
{
struct RenderImageStateWrapper
{
    std::unique_ptr<PixelariumImageView> view;
    const bool* show_state;
};

class RenderImageManager
{
    using Pool = resources::ImageResourcePool;

   public:
    explicit RenderImageManager(Pool& pool, const utils::log::ILog& log)
        : view_factory_(std::make_unique<ImageViewFactory>(pool)), log_(log)
    {
    }

    void Clear() noexcept;

    void Add(resources::ResourceKey key) noexcept;

    bool Remove(resources::ResourceKey key) noexcept;

    // can't be const because func mutates the state
    template <typename Callable>
        requires std::invocable<Callable, resources::ResourceKey, RenderImageStateWrapper&>
    void Enumerate(Callable&& func)
    {
        for (auto& [key, val] : this->render_image_map_)
        {
            if (val.view != nullptr)
            {
                func(key, val);
            }
        }
    }

    void MarkForDeletion(resources::ResourceKey key);

    void UpdateCollection();

   private:
    std::unordered_map<resources::ResourceKey, RenderImageStateWrapper> render_image_map_;
    std::unique_ptr<ImageViewFactory> view_factory_;
    std::mutex mut_;
    std::unordered_set<resources::ResourceKey> keys_to_delete_;

    const utils::log::ILog& log_;
};
}  // namespace pixelarium::ui
