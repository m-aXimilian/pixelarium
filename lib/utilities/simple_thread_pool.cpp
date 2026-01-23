#include "simple_thread_pool.hpp"

#include <functional>
#include <mutex>

using namespace pixelarium::utils;

simple_thread_pool::simple_thread_pool(size_t num_threads)
{
    for (size_t i{0}; i < num_threads; ++i)
    {
        workers_.emplace_back(
            [this]()
            {
                while (true)
                {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lck(thread_mutex_);

                        cv_.wait(lck, [this]() -> bool { return shutdown_ || !task_queue_.empty(); });

                        if (shutdown_ && task_queue_.empty()) return;

                        job = std::move(task_queue_.front());
                        task_queue_.pop();
                    }

                    job();
                }
            });
    }
}

simple_thread_pool::~simple_thread_pool()
{
    {
        std::unique_lock<std::mutex> lck(thread_mutex_);
        shutdown_ = true;
    }

    cv_.notify_all();
    for (auto& th : workers_)
    {
        th.join();
    }
}
