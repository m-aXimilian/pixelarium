#include "RenderImageManager.hpp"
#include <format>

namespace px = pixelarium;
using namespace std;

void pixelarium::render::RenderImageManager::UpdateCollection()
{
    // this->log_.Debug(std::format("{}: Updating collection", __PRETTY_FUNCTION__));
    // here we must not lock the mutex because the Remove function will also lock it -> deadlock!
    // lock_guard<mutex> guard(this->mut_);
    for (const auto& key : keys_to_delete_)
    {
        this->Remove(key);
    }

    keys_to_delete_.clear();
}
void pixelarium::render::RenderImageManager::MarkForDeletion(resources::ResourceKey key)
{
    this->log_.Debug(std::format("{} marking key: \"{}\" for deletion.", __PRETTY_FUNCTION__, key));
    lock_guard<mutex> guard(this->mut_);
    keys_to_delete_.insert(key);
}
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
void pixelarium::render::RenderImageManager::Add(resources::ResourceKey key) noexcept
{
    // we don't want to add what's already there
    // or empty render images
    auto current_view = this->view_factory_->RenderImage(key);
    if (this->render_image_map_.contains(key) ||
        current_view == nullptr)
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
                    .show_state = current_view->GetStatus(),
                }
            }
            );
    // clang-format on
}
void pixelarium::render::RenderImageManager::Clear() noexcept { this->render_image_map_.clear(); }
