#include "resource.hpp"

#include <atomic>
#include <functional>
#include <optional>

using pixelarium::imaging::PixelariumImage;
using namespace std;

namespace
{
/// @brief Atomic counter for generating unique IDs.
static std::atomic<size_t> id_;

/// @brief Generates a unique ID.
/// @return A unique ID.
size_t GenerateId() { return id_.fetch_add(1, memory_order_relaxed); }
}  // namespace

/// @brief Retrieves a resource from the pool.
/// @param id The ID of the resource to retrieve.
/// @return A pointer to the resource if found, otherwise an empty optional.
std::optional<const PixelariumImage*> pixelarium::resources::ImageResourcePool::GetResource(size_t id) const
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return std::nullopt;

    return search->second.get();
}

/// @brief Sets a resource in the pool.
/// @param res A unique pointer to the resource to set.
/// @return The ID of the new resource.
size_t pixelarium::resources::ImageResourcePool::SetResource(unique_ptr<PixelariumImage> res)
{
    auto key{::GenerateId()};
    this->resources_.insert({key, std::move(res)});

    return key;
}

/// @brief Updates a resource in the pool.
/// @param id The ID of the resource to update.
/// @param res A unique pointer to the new resource.
/// @return True if the resource was updated, false otherwise.
bool pixelarium::resources::ImageResourcePool::UpdateResource(size_t id, std::unique_ptr<imaging::PixelariumImage> res)
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return false;

    search->second = std::move(res);

    return true;
}

/// @brief Deletes a resource from the pool.
/// @param id The ID of the resource to delete.
/// @return True if the resource was deleted, false otherwise.
bool pixelarium::resources::ImageResourcePool::DeleteResource(size_t id)
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return false;

    this->resources_.erase(search);

    return true;
}

/// @brief Enumerates all resources in the pool.
/// @param func A function to call for each resource.  The function should accept the resource ID and a const reference
/// to a PixelariumImage.
void pixelarium::resources::ImageResourcePool::EnumerateResources(
    const std::function<void(size_t, const imaging::PixelariumImage&)>& func)
{
    for (const auto& e : this->resources_)
    {
        func(e.first, *e.second);
    }
}
