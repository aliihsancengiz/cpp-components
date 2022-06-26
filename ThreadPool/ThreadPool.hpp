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
struct ThreadPool
{
    using Work = std::function<void(int)>;
    ThreadPool(std::size_t threadCount = 4);
    void dispatch(Work&& w);
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
