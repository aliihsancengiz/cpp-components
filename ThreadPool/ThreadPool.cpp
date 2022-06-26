#include "ThreadPool.hpp"

namespace thread_pool {
ThreadPool::ThreadPool(std::size_t threadCount) : numberOfThread(threadCount)
{
    isStop.store(false);
    for (size_t i = 0; i < numberOfThread; i++) {
        mThreadList.emplace_back([this]() {
            while (!isStop) {
                Work w = nullptr;
                {
                    std::unique_lock<std::mutex> lock(mWorkQueueMutex);
                    mWorkQueueNotifier.wait(lock, [this]() {
                        return !mWorkQueue.empty() || isStop;
                    });
                    if (!isStop) {

                        w = mWorkQueue.front();
                        mWorkQueue.pop();
                    }
                }
                if (!isStop && w != nullptr) {
                    w();
                }
            }
        });
    }
}
void ThreadPool::dispatch(Work&& w)
{
    mWorkQueue.emplace(w);
    mWorkQueueNotifier.notify_one();
}
ThreadPool::~ThreadPool()
{
    isStop.store(true);
    mWorkQueueNotifier.notify_all();
    for (size_t i = 0; i < numberOfThread; i++) {
        mThreadList[i].join();
    }
}

}  // namespace thread_pool
