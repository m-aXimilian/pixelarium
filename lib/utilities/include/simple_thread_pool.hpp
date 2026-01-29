#pragma once

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
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

    [[nodiscard]]
    decltype(auto) RunningTasks() const
    {
        return running_tasks_.load();
    }

    decltype(auto) Joinable() const
    {
        std::unique_lock<std::mutex> lck(thread_mutex_);

        return task_queue_.empty() && RunningTasks() == 0;
    }

    [[nodiscard]]
    static decltype(auto) GlobalRunningTasks()
    {
        return Global().RunningTasks();
    }

    static decltype(auto) GlobalJoinable() { return Global().Joinable(); }

   private:
    static auto Global() -> simple_thread_pool&
    {
        const auto kThreadCount{std::thread::hardware_concurrency() * 2};
        static simple_thread_pool global_instance(kThreadCount == 0 ? 5 : kThreadCount);
        return global_instance;
    }
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::mutex mutable thread_mutex_;
    std::queue<std::function<void()>> task_queue_;
    bool shutdown_{false};
    std::atomic_size_t running_tasks_{0};
};
}  // namespace pixelarium::utils
