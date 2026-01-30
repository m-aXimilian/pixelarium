#pragma once

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
template <size_t N>
class simple_thread_pool
{
   public:
    simple_thread_pool()
    {
        static_assert(N > 0, "Pool must have at least one thread.");
        for (size_t i{0}; i < N; ++i)
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
                            ++running_tasks_;
                        }

                        job();
                        --running_tasks_;
                    }
                });
        }
    }

    simple_thread_pool(simple_thread_pool&) = delete;
    simple_thread_pool(const simple_thread_pool&) = delete;
    simple_thread_pool(simple_thread_pool&&) = delete;
    simple_thread_pool& operator=(simple_thread_pool&) = delete;
    simple_thread_pool& operator=(simple_thread_pool&&) = delete;
    ~simple_thread_pool()
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

   private:
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::mutex mutable thread_mutex_;
    std::queue<std::function<void()>> task_queue_;
    bool shutdown_{false};
    std::atomic_size_t running_tasks_{0};
};

class pixelarium_pool
{
   public:
    [[nodiscard]]
    static decltype(auto) RunningTasks()
    {
        return pixelarium_pool::instance().RunningTasks();
    }

    static decltype(auto) Joinable() { return pixelarium_pool::instance().Joinable(); }

    template <typename Callable>
        requires std::invocable<Callable>
    static auto enqueue(Callable&& fun) -> void
    {
        pixelarium_pool::instance().enqueue(std::forward<Callable>(fun));
    }

   private:
    static constexpr auto kThreadCount{20};
    static simple_thread_pool<kThreadCount>& instance()
    {
        static simple_thread_pool<kThreadCount> pixelarium_pool_instance;
        return pixelarium_pool_instance;
    }
};
}  // namespace pixelarium::utils
