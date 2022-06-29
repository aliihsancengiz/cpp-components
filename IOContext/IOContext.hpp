#pragma once
#include "IOMultiplexer.hpp"
#include "Logger.hpp"
#include "ThreadPool.hpp"

namespace io_context {
struct IOContext
{
  public:
    IOContext(int numberOfThreads = 4);
    ~IOContext();
    void post(thread_pool::Work&& w);
    template<typename Func, typename... Args>
    void post(Func&& work, Args&&... args)
    {
        mLogger->trace("New Work Posted to Thread pool");
        pool->dispatchWithArgs(std::forward<Func>(work), std::forward<Args>(args)...);
    }
    void run();

  private:
    std::unique_ptr<thread_pool::ThreadPool> pool;
    std::shared_ptr<spdlog::logger> mLogger;
    int threadCount;
};

}  // namespace io_context
