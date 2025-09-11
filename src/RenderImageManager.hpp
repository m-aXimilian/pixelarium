#pragma once
#include <memory>
#include <unordered_map>

#include "PixelariumImage.hpp"
#include "resources/resource.hpp"
#include "viewmodels/ImageViewFactory.hpp"
#include "views/PixelariumImageView.hpp"

// This is intended as an additional abstraction
// aggregating views that should be rendered (or not)
namespace pixelarium::ui
{
struct RenderImageStateWrapper
{
    std::unique_ptr<PixelariumImageView> view;
    bool show_state;
};

class RenderImageManager
{
    using Pool = resources::ImageResourcePool;

   public:
    explicit RenderImageManager(Pool& pool);

    void Clear() noexcept { this->render_image_map_.clear(); }

    void Add(resources::ResourceKey key) noexcept
    {
        // clang-format off
        this->render_image_map_.insert(
            {
                key,
                RenderImageStateWrapper
                {
                    .view{ this->view_factory_->RenderImage(key) },
                    .show_state = false
                }
            }
            );
        // clang-format on
    }

    bool Remove(resources::ResourceKey key) noexcept
    {
        if (this->render_image_map_.erase(key) == 1)
        {
            return true;
        }

        return false;
    }

    // can't be const because func mutates the state
    template <typename Callable>
        requires std::invocable<Callable, resources::ResourceKey, RenderImageStateWrapper&>
    void Enumerate(Callable&& func)
    {
        for (auto& [key, val] : this->render_image_map_)
        {
            func(key, val);
        }
    }

   private:
    std::unordered_map<resources::ResourceKey, RenderImageStateWrapper> render_image_map_;
    std::unique_ptr<ImageViewFactory> view_factory_;
};
}  // namespace pixelarium::ui
