#include <gtest/gtest.h>

#include <chrono>
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
        pixelarium_pool::enqueue(
            [&mutate, i]()
            {
                const auto wait = std::chrono::milliseconds(i);
                std::this_thread::sleep_for(wait);
                mutate(i, i);
            });
    }

    // wait until each spawned task finished
    while (!pixelarium_pool::Joinable()) std::this_thread::sleep_for(5ms);

    EXPECT_EQ(mutation_vector.size(), vecsize);
    for (auto i{0}; i < vecsize; ++i)
    {
        EXPECT_EQ(mutation_vector.at(i), i);
    }
}

// TEST(SimpleThreadPoolTest, ZeroThreads)
// {
//     using namespace pixelarium::utils;
//     // Creating a thread pool with 0 threads should not throw an exception
//     // and should be joinable immediately.
//     simple_thread_pool<0> pool;
//     EXPECT_TRUE(pool.Joinable());

//     // Enqueuing a task on a 0-thread pool should not crash.
//     // The task will never run, but the enqueue operation should be valid.
//     pool.enqueue([]() {});
//     EXPECT_FALSE(pool.Joinable()); // Now it's not joinable because there's a task
//     EXPECT_EQ(pool.RunningTasks(), 0); // No tasks are running

//     // Destructor should also handle the 0-thread case gracefully.
// }
