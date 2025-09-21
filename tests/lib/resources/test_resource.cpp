#include <gtest/gtest.h>

#include <algorithm>

#include "imaging/PixelariumImage.hpp"
#include "resources/resource.hpp"

namespace
{

class DummyImage : public pixelarium::imaging::PixelariumImage
{
    // Implement minimal interface if needed for test
};

}  // anonymous namespace

using pixelarium::resources::ImageResourcePool;

TEST(ImageResourcePoolTest, SetAndGetResource)
{
    ImageResourcePool pool;
    auto img = std::make_unique<DummyImage>();
    auto id = pool.SetResource(std::move(img));
    auto res = pool.GetResource(id);
    EXPECT_TRUE(res.has_value());
    EXPECT_NE(res.value(), nullptr);
}

TEST(ImageResourcePoolTest, SetWrappedRawPointerGet)
{
    ImageResourcePool pool;
    auto img = new DummyImage();
    auto id = pool.SetResource(std::unique_ptr<pixelarium::imaging::PixelariumImage>(img));
    auto res = pool.GetResource(id);
    EXPECT_TRUE(res.has_value());
    EXPECT_NE(res.value(), nullptr);
}

TEST(ImageResourcePoolTest, GetNonExistentResourceReturnsEmptyOptional)
{
    ImageResourcePool pool;
    EXPECT_FALSE(pool.GetResource(12345));
}

TEST(ImageResourcePoolTest, ModifyResourceSuccess)
{
    ImageResourcePool pool;
    auto id = pool.SetResource(std::make_unique<DummyImage>());
    auto new_img = std::make_unique<DummyImage>();
    EXPECT_TRUE(pool.ModifyResource(id, std::move(new_img)));
    auto res = pool.GetResource(id);
    EXPECT_TRUE(res.has_value());
    EXPECT_NE(res.value(), nullptr);
}

TEST(ImageResourcePoolTest, ModifyResourceFail)
{
    ImageResourcePool pool;
    auto new_img = std::make_unique<DummyImage>();
    EXPECT_FALSE(pool.ModifyResource(999, std::move(new_img)));
}

TEST(ImageResourcePoolTest, DeleteResourceSuccess)
{
    ImageResourcePool pool;
    auto id = pool.SetResource(std::make_unique<DummyImage>());
    EXPECT_TRUE(pool.DeleteResource(id));
    EXPECT_FALSE(pool.GetResource(id).has_value());
}

TEST(ImageResourcePoolTest, DeleteResourceFail)
{
    ImageResourcePool pool;
    EXPECT_FALSE(pool.DeleteResource(8907));
}

TEST(ImageResourcePoolTest, EnumerateResources)
{
    ImageResourcePool pool;
    auto id1 = pool.SetResource(std::make_unique<DummyImage>());
    auto id2 = pool.SetResource(std::make_unique<DummyImage>());
    std::vector<size_t> found_ids{};

    pool.EnumerateResources([&found_ids](size_t id, size_t, const pixelarium::imaging::PixelariumImage&) { found_ids.push_back(id); });

    EXPECT_EQ(found_ids.size(), 2);
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id1), found_ids.end());
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id2), found_ids.end());
}

TEST(ImageResourcePoolTest, TemplatedEnumerate)
{
    ImageResourcePool pool;
    auto id1 = pool.SetResource(std::make_unique<DummyImage>());
    auto id2 = pool.SetResource(std::make_unique<DummyImage>());
    std::vector<size_t> found_ids{};

    pool.Enumerate([&found_ids](size_t id, size_t, const pixelarium::imaging::PixelariumImage&) { found_ids.push_back(id); });

    EXPECT_EQ(found_ids.size(), 2);
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id1), found_ids.end());
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id2), found_ids.end());
}
