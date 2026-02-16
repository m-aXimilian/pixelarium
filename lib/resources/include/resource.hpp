#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "IPixelariumImage.hpp"

namespace pixelarium::resources
{
using ResourceKey = size_t;

/// @brief A dedicated exception to be thrown when a resource of an IResourcePool is empty.
struct empty_resource_exception : public std::exception
{
    empty_resource_exception() {};
    empty_resource_exception(std::string& msg) : message_(msg) {};
    const char* what() const noexcept { return message_.c_str(); }

   private:
    std::string message_ = "Empty Resource";
};

/// @brief Abstract representation of a Resource.
/// This is meant to be implemented by arbitrary explicit resource types and thus
/// gives no contract other than the abstract type.
struct IResource
{
    virtual ~IResource() = default;
};

/// @brief Defines a concept for a resource type
/// @tparam R  The resource template parameter
template <typename R>
concept ResT = requires(R& r) { static_cast<IResource&>(r); };

template <typename T>
struct pool_traits;

/// @brief Defines an interface for a resource pool
/// @tparam ResT  defines the resource type that is accepted by the pool
template <typename R, class D>
class IResourcePool
{
   public:
    virtual ~IResourcePool() = default;
    virtual std::weak_ptr<R> GetResource(size_t id) const = 0;
    virtual ResourceKey SetResource(std::unique_ptr<R> res) = 0;
    virtual bool ModifyResource(ResourceKey id, std::unique_ptr<R> res) = 0;
    virtual bool DeleteResource(ResourceKey id) = 0;
    virtual void EnumerateResources(
        const std::function<void(ResourceKey, size_t, const imaging::IPixelariumImage<D>&)>& func) = 0;
    virtual size_t GetTotalSize() const = 0;
    virtual void Clear() = 0;
};

// Now with the =GetResource= method, I do not want to transfer ownership to the caller of that method. The ownership
// should still
// reside with the =ResourcePool=!
// In fact, the intention is, that there is no way back once the =ResourcePool= took ownership of an object.
// Callers can get references, but no ownership. A caller might delete a resource though.
class ImageResourcePool : public IResourcePool<imaging::IPixelariumImageCvMat, cv::Mat>
{
   public:
    ImageResourcePool() = default;
    ImageResourcePool(ImageResourcePool&) = delete;
    ImageResourcePool(const ImageResourcePool&) = delete;
    ImageResourcePool(ImageResourcePool&&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&) = delete;
    ImageResourcePool& operator=(ImageResourcePool&&) = delete;

    std::weak_ptr<imaging::IPixelariumImageCvMat> GetResource(ResourceKey id) const override;
    ResourceKey SetResource(std::unique_ptr<imaging::IPixelariumImageCvMat> res) override;
    bool ModifyResource(ResourceKey id, std::unique_ptr<imaging::IPixelariumImageCvMat> res) override;
    bool DeleteResource(ResourceKey id) override;
    void Clear() override { this->resources_.clear(); }

    void EnumerateResources(
        const std::function<void(ResourceKey, size_t, const imaging::IPixelariumImage<cv::Mat>&)>& func) override;

    template <typename Callable>
        requires std::invocable<Callable, ResourceKey, size_t, const imaging::IPixelariumImageCvMat&>
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
    std::unordered_map<size_t, std::shared_ptr<imaging::IPixelariumImageCvMat>> resources_;
    std::mutex mut_;
};

template <typename R, typename D>
struct pool_traits<IResourcePool<R, D>>
{
    using resource_type = R;
    using data_type = D;
};

template <>
struct pool_traits<ImageResourcePool>
{
    using resource_type = imaging::IPixelariumImageCvMat;
    using data_type = cv::Mat;
};
}  // namespace pixelarium::resources
