#pragma once

#include <atomic>
#include <unordered_map>

namespace
{
class UniqueKeyGenerator
{
   public:
    static auto GenerateKey() { return _counter.fetch_add(1, std::memory_order_relaxed); }

   private:
    static std::atomic<std::size_t> _counter;
};
}

namespace pixelarium::resources
{

template <class Core>
class IResource
{
   public:
    virtual const Core& GetCore() const = 0;

    virtual ~IResource() = 0;
};

class IResourcePool
{
public:
    virtual ~IResourcePool() = 0;
}

template <class Core>
class ResourcePool : IResourcePool
{
   public:
    auto AddResource(IResource<Core>& res)
    {
        const auto key {::UniqueKeyGenerator::GenerateKey()};
        this->_resources.insert(key, std::move(res));

        return key;
    }

   private:
    std::unordered_map<size_t, Core> _resources;
};

class ResourceManger
{
public:
    template <typename Core>
    auto AddResource(IResource<Core>)
}

}  // namespace pixelarium::resources