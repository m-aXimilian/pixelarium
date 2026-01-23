#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace pixelarium::utils
{
class simple_thread_pool
{
   public:
    explicit simple_thread_pool(size_t);
    simple_thread_pool(simple_thread_pool&) = delete;
    simple_thread_pool(const simple_thread_pool&) = delete;
    simple_thread_pool(simple_thread_pool&&) = delete;
    simple_thread_pool& operator=(simple_thread_pool&) = delete;
    simple_thread_pool& operator=(simple_thread_pool&&) = delete;
    ~simple_thread_pool();

    template <typename Callable>
        requires std::invocable<Callable>
    static auto run_asynch(Callable&& fun) -> void
    {
        simple_thread_pool::Global().enqueue(std::forward<Callable>(fun));
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
    static auto Global() -> simple_thread_pool&
    {
        const auto kThreadCount{std::thread::hardware_concurrency() * 2};
        static simple_thread_pool global_instance(kThreadCount == 0 ? 5 : kThreadCount);
        return global_instance;
    }
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::mutex thread_mutex_;
    std::queue<std::function<void()>> task_queue_;
    bool shutdown_{false};
};
}  // namespace pixelarium::utils
