#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace pixelarium::utils
{
class thread_pool
{
   public:
    explicit thread_pool(size_t);
    thread_pool(thread_pool&) = delete;
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(thread_pool&) = delete;
    thread_pool& operator=(thread_pool&&) = delete;
    ~thread_pool();

    template <typename Callable>
        requires std::invocable<Callable>
    static auto run_asynch(Callable&& fun) -> void
    {
        thread_pool::Global().enqueue(std::forward<Callable>(fun));
    }

   public:
    template <typename Callable>
        requires std::invocable<Callable>
    auto enqueue(Callable&& fun) -> void
    {
        {
            std::unique_lock<std::mutex> lck(thread_mutex_);
            task_queue_.emplace(std::forward<Callable>(fun));
        }

        cv_.notify_one();
    }

   private:
    static auto Global() -> thread_pool&
    {
        const auto kThreadCount{std::thread::hardware_concurrency() * 2};
        static thread_pool global_instance(kThreadCount == 0 ? 5 : kThreadCount);
        return global_instance;
    }
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::mutex thread_mutex_;
    std::queue<std::function<void()>> task_queue_;
    bool shutdown_{false};
};
}  // namespace pixelarium::utils
