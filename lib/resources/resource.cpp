#include "resource.hpp"

#include <atomic>

using pixelarium::imaging::PixelariumImage;
using namespace std;

namespace
{
// struct UniqueKeyGenerator
// {
//     static auto GenerateKey() { return counter_.fetch_add(1, memory_order_relaxed); }

//    private:
//     static std::atomic<std::size_t> counter_;
// };

static std::atomic<size_t> id_;
size_t GenerateId() { return id_.fetch_add(1, memory_order_relaxed); }
}  // namespace

std::optional<const PixelariumImage*> pixelarium::resources::ImageResourcePool::GetResource(size_t id) const
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return {};

    return search->second.get();
}

size_t pixelarium::resources::ImageResourcePool::SetResource(unique_ptr<PixelariumImage> res)
{
    // auto key{UniqueKeyGenerator::GenerateKey()};
    auto key{::GenerateId()};
    this->resources_.insert({key, std::move(res)});

    return key;
}

bool pixelarium::resources::ImageResourcePool::UpdateResource(size_t id, std::unique_ptr<imaging::PixelariumImage> res)
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return false;

    this->resources_.at(id) = std::move(res);

    return true;
}

bool pixelarium::resources::ImageResourcePool::DeleteResource(size_t id)
{
    auto search{this->resources_.find(id)};
    if (search == this->resources_.end()) return false;

    this->resources_.erase(search);

    return true;
}
