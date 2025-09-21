#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "imaging/IPixelariumImage.hpp"

namespace pixelarium::resources
{
    using ResourceKey = size_t;
struct IResource
{
    virtual ~IResource() = default;
};

template <typename R>
concept ResT = requires(R& r) { static_cast<IResource&>(r); };

template <typename ResT>
class IResourcePool
{
   public:
    virtual ~IResourcePool() = default;
    virtual std::optional<std::weak_ptr<ResT>> GetResource(size_t id) const = 0;
    virtual ResourceKey SetResource(std::unique_ptr<ResT> res) = 0;
    virtual bool ModifyResource(ResourceKey id, std::unique_ptr<ResT> res) = 0;
    virtual bool DeleteResource(ResourceKey id) = 0;
    virtual void EnumerateResources(const std::function<void(ResourceKey, size_t, const imaging::IPixelariumImage&)>& func) = 0;
    virtual size_t GetTotalSize() const = 0;
};

// Now with the =GetResource= method, I do not want to transfer ownership to the caller of that method. The ownership
// should still
// reside with the =ResourcePool=!
// In fact, the intention is, that there is no way back once the =ResourcePool= took ownership of an object.
// Callers can get references, but no ownership. A caller might delete a resource though.
class ImageResourcePool : public IResourcePool<imaging::IPixelariumImage>
{
   public:
    ImageResourcePool() = default;
    ImageResourcePool(ImageResourcePool&) = delete;
    ImageResourcePool(const ImageResourcePool&) = delete;
    ImageResourcePool(ImageResourcePool&&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&&) = delete;

    std::optional<std::weak_ptr<imaging::IPixelariumImage>> GetResource(ResourceKey id) const override;
    ResourceKey SetResource(std::unique_ptr<imaging::IPixelariumImage> res) override;
    bool ModifyResource(ResourceKey id, std::unique_ptr<imaging::IPixelariumImage> res) override;
    bool DeleteResource(ResourceKey id) override;

    void EnumerateResources(const std::function<void(ResourceKey, size_t, const imaging::IPixelariumImage&)>& func) override;

    template <typename Callable>
    requires std::invocable<Callable, ResourceKey, size_t, const imaging::IPixelariumImage&>
    void Enumerate(Callable&& func) const
    {
        size_t idx{0};
        for (const auto& e : this->resources_)
        {
            func(e.first, idx, *e.second);
        }
    }

    size_t GetTotalSize() const override { return resources_.size(); }

   private:
    std::unordered_map<size_t, std::shared_ptr<imaging::IPixelariumImage>> resources_;
    std::mutex mut_;
};
}  // namespace pixelarium::resources
