#include <gtest/gtest.h>

#include <algorithm>

#include "imaging/IPixelariumImage.hpp"
#include "resources/resource.hpp"

namespace
{

// A Mock implementation for tests requiring _any_ instance of a IPixelariumImage
class DummyImage : public pixelarium::imaging::IPixelariumImageCvMat
{
   public:
    std::optional<cv::Mat> TryGetImage() override { return {}; }

    std::optional<cv::Mat> TryGetImage(const pixelarium::imaging::IImageQuery&) override { return {}; }

    std::vector<std::optional<cv::Mat>> TryGetImages(const pixelarium::imaging::IImageQuery&) override { return {}; }

    std::string Name() const noexcept override { return {}; }

    bool Empty() const noexcept override { return true; }

    std::filesystem::path Uri() const noexcept override { return {}; }
};

}  // anonymous namespace

using pixelarium::resources::ImageResourcePool;

TEST(ImageResourcePoolTest, SetAndGetResource)
{
    ImageResourcePool pool;
    auto img = std::make_unique<DummyImage>();
    auto id = pool.SetResource(std::move(img));
    auto res = pool.GetResource(id);
    auto res_img = res.lock();

    EXPECT_NE(res_img, nullptr);
}

TEST(ImageResourcePoolTest, SetWrappedRawPointerGet)
{
    ImageResourcePool pool;
    auto img = new DummyImage();
    auto id = pool.SetResource(std::unique_ptr<pixelarium::imaging::IPixelariumImageCvMat>(img));
    auto res = pool.GetResource(id);
    auto res_img = res.lock();
    EXPECT_NE(res_img, nullptr);
}

TEST(ImageResourcePoolTest, GetNonExistentResourceReturnsEmptyOptional)
{
    ImageResourcePool pool;
    EXPECT_EQ(pool.GetResource(12345).lock(), nullptr);
}

TEST(ImageResourcePoolTest, ModifyResourceSuccess)
{
    ImageResourcePool pool;
    auto id = pool.SetResource(std::make_unique<DummyImage>());
    auto new_img = std::make_unique<DummyImage>();
    EXPECT_TRUE(pool.ModifyResource(id, std::move(new_img)));
    auto res = pool.GetResource(id);
    auto res_img = res.lock();

    EXPECT_NE(res_img, nullptr);
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
    EXPECT_EQ(pool.GetResource(id).lock(), nullptr);
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

    pool.EnumerateResources([&found_ids](size_t id, size_t, const pixelarium::imaging::IPixelariumImage<cv::Mat>&)
                            { found_ids.push_back(id); });

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

    pool.Enumerate([&found_ids](size_t id, size_t, const pixelarium::imaging::IPixelariumImage<cv::Mat>&)
                   { found_ids.push_back(id); });

    EXPECT_EQ(found_ids.size(), 2);
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id1), found_ids.end());
    EXPECT_NE(std::find(found_ids.begin(), found_ids.end(), id2), found_ids.end());
}
