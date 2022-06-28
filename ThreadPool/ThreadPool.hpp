#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace thread_pool {
using Work = std::function<void()>;

struct ThreadPool
{
    ThreadPool(std::size_t threadCount = 4);
    void dispatch(Work&& w);
    template<typename Func, typename... Args>
    void dispatchWithArgs(Func&& work, Args&&... args)
    {
        mWorkQueue.emplace(std::bind(std::forward<Func>(work), std::forward<Args>(args)...));
        mWorkQueueNotifier.notify_one();
    }
    ~ThreadPool();

  private:
    std::size_t numberOfThread;
    std::queue<Work> mWorkQueue;
    std::mutex mWorkQueueMutex;
    std::condition_variable mWorkQueueNotifier;
    std::vector<std::thread> mThreadList;
    std::atomic<bool> isStop;
};
}  // namespace thread_pool
