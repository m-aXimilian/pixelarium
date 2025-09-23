#include "RenderImageManager.hpp"

#include <format>

using namespace std;

/// @brief Updates the collection of rendered images by removing images marked for deletion.
/// This function iterates through the \c keys_to_delete_ list and removes the corresponding images from the collection.
/// It does not acquire the mutex to avoid deadlocks with the `Remove` function.
void pixelarium::render::RenderImageManager::UpdateCollection()
{
    for (const auto& key : keys_to_delete_)
    {
        this->Remove(key);
    }

    keys_to_delete_.clear();
}

/// @brief Marks a resource for deletion.
/// @param key The ID of the resource to mark for deletion.
void pixelarium::render::RenderImageManager::MarkForDeletion(resources::ResourceKey key)
{
    this->log_.Debug(std::format("{} marking key: \"{}\" for deletion.", __PRETTY_FUNCTION__, key));
    lock_guard<mutex> guard(this->mut_);
    keys_to_delete_.insert(key);
}

/// @brief Removes a render image from the manager.
/// @param key The key of the render image to remove.
/// @return True if the render image was removed, false otherwise.
bool pixelarium::render::RenderImageManager::Remove(resources::ResourceKey key) noexcept
{
    bool remove_state{false};
    this->log_.Debug(std::format("{} removing key: \"{}\" from renderlist.", __PRETTY_FUNCTION__, key));
    {
        lock_guard<mutex> guard(this->mut_);
        remove_state = this->render_image_map_.erase(key) == 1;
    }

    return remove_state;
}

/// @brief Adds a resource to the render image map.
/// @param key The ID of the resource to add.
/// @return void.  No exception is thrown.
void pixelarium::render::RenderImageManager::Add(resources::ResourceKey key) noexcept
{
    // we don't want to add what's already there
    // or empty render images
    auto current_view = this->view_factory_->RenderImage(key);
    if (this->render_image_map_.contains(key) || current_view == nullptr)
    {
        return;
    }

    this->log_.Debug(std::format("{}: adding key: \"{}\" to renderlist.", __PRETTY_FUNCTION__, key));

    lock_guard<mutex> guard(this->mut_);
    // clang-format off
        this->render_image_map_.insert(
            {
                key,
                RenderImageStateWrapper
                {
                    .view{ std::move(current_view) },
                }
            }
            );
    // clang-format on
}

/// @brief Clears all render images from the manager.
/// @note This function is noexcept.
void pixelarium::render::RenderImageManager::Clear() noexcept { this->render_image_map_.clear(); }
