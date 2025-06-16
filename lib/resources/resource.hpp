#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "imaging/PixelariumImage.hpp"

namespace pixelarium::resources
{
struct IResource
{
    virtual ~IResource() = default;
};

template <typename R>
concept ResT = requires(R& r) { static_cast<IResource&>(r); };

// template <ResT R>
template <typename R>
class IResourcePool
{
   public:
    virtual ~IResourcePool() = default;
    virtual std::optional<const R*> GetResource(size_t id) const = 0;
    virtual size_t SetResource(std::unique_ptr<R> res) = 0;
    virtual bool ModifyResource(size_t id, std::unique_ptr<R> res) = 0;
    virtual bool DeleteResource(size_t id) = 0;
    virtual void EnumerateResources(const std::function<void(size_t, const R&)>& func) = 0;
    virtual size_t GetTotalSize() const = 0;
};

// Now with the =GetResource= method, I do not want to transfer ownership to the caller of that method. The ownership
// should still
// reside with the =ResourcePool=!
// In fact, the intention is, that there is no way back once the =ResourcePool= took ownership of an object.
// Callers can get references, but no ownership. A caller might delete a resource though.
class ImageResourcePool : public IResourcePool<imaging::PixelariumImage>
{
   public:
    ImageResourcePool() = default;
    ImageResourcePool(ImageResourcePool&) = delete;
    ImageResourcePool(const ImageResourcePool&) = delete;
    ImageResourcePool(ImageResourcePool&&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&&) = delete;

    std::optional<const imaging::PixelariumImage*> GetResource(size_t id) const override;
    size_t SetResource(std::unique_ptr<imaging::PixelariumImage> res) override;
    bool ModifyResource(size_t id, std::unique_ptr<imaging::PixelariumImage> res) override;
    bool DeleteResource(size_t id) override;

    void EnumerateResources(const std::function<void(size_t, const imaging::PixelariumImage&)>& func) override;

    size_t GetTotalSize() const override { return resources_.size();}
   private:
    std::unordered_map<size_t, std::unique_ptr<imaging::PixelariumImage>> resources_;
};
}  // namespace pixelarium::resources
