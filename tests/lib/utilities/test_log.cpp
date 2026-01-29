#include <gtest/gtest.h>

#include <chrono>
#include <print>
#include <thread>
#include <vector>

#include "simple_thread_pool.hpp"

TEST(SimpleThreadPoolTest, MutatesFromDifferentThread)
{
    using namespace std::chrono_literals;
    using namespace pixelarium::utils;
    std::mutex mut;
    constexpr auto vecsize{10};
    std::vector<int> mutation_vector(vecsize, 0);

    auto mutate = [&mutation_vector](int val, int loc) -> void { mutation_vector.at(loc) = val; };

    for (auto i{0}; i < vecsize; ++i)
    {
        pixelarium::utils::simple_thread_pool::run_asynch(
            [&mutate, i]()
            {
                const auto wait = std::chrono::milliseconds(i);
                std::this_thread::sleep_for(wait);
                mutate(i, i);
            });
    }

    // wait until each spawned task finished
    while (!simple_thread_pool::GlobalJoinable()) std::this_thread::sleep_for(5ms);

    EXPECT_EQ(mutation_vector.size(), vecsize);
    for (auto i{0}; i < vecsize; ++i)
    {
        EXPECT_EQ(mutation_vector.at(i), i);
    }
}
