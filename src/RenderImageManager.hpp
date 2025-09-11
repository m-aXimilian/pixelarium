#pragma once
#include <cstdio>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

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
        std::lock_guard<std::mutex> guard(this->mut_);
        // clang-format off
        this->render_image_map_.insert(
            {
                key,
                RenderImageStateWrapper
                {
                    .view{ this->view_factory_->RenderImage(key) },
                    .show_state = true
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

    void MarkForDeletion(resources::ResourceKey key)
    {
        std::lock_guard<std::mutex> guard(this->mut_);
        keys_to_delete_.push_back(key);
    }

    void UpdateCollection()
    {
        // std::erase_if(this->render_image_map_,
        //               [](const auto& el)
        //               {
        //                   const auto& [k, v] = el;
        //                   return k == false;
        //               });
        std::lock_guard<std::mutex> guard(this->mut_);
        for (const auto& key : keys_to_delete_)
        {
            this->render_image_map_.erase(key);
        }

        keys_to_delete_.clear();
    }

private:
    std::unordered_map<resources::ResourceKey, RenderImageStateWrapper> render_image_map_;
    std::unique_ptr<ImageViewFactory> view_factory_;
    std::mutex mut_;
    std::list<resources::ResourceKey> keys_to_delete_;
};
}  // namespace pixelarium::ui
