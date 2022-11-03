#pragma once

#include "Logger.hpp"
#include "ThreadPool.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace io_multiplexer {

struct ioObject
{
    using fd_t = std::int32_t;
    ioObject(fd_t fd) : fd(fd) {}
    ioObject() {}
    fd_t getUnderlyingFileDescriptor() const
    {
        return fd;
    }
    void setAsNonBlocking()
    {
        int flags = fcntl(fd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(fd, F_SETFL, flags);
    }

  protected:
    fd_t fd{};
};

enum class EventType : int64_t
{
    READ = EPOLLIN,
    WRITE = EPOLLOUT,
    ERROR = EPOLLERR,
    EDGETRIGERRED = EPOLLET
};

using Callback = std::function<void(ioObject&, EventType)>;

struct IOMultiplexer
{

  private:
    using EventHandlerMap = std::map<std::int32_t, std::map<EventType, Callback>>;
    typedef struct epoll_event epool_event;
    constexpr static int MAX_EVENTS = 255;

  public:
    IOMultiplexer(std::shared_ptr<thread_pool::ThreadPool>& mPoolPtr);

    IOMultiplexer(const IOMultiplexer&) = delete;
    IOMultiplexer& operator=(const IOMultiplexer&) = delete;

    ~IOMultiplexer();

    void registerEvent(const ioObject& ioObj, const EventType& type, Callback cb);
    void deregisterEvent(const ioObject& ioObj, const EventType& type);

    void pollForIO();

  private:
    EventHandlerMap mEventHandlerMap;
    std::int32_t mEpollFd;
    std::shared_ptr<spdlog::logger> mLogger;
    std::shared_ptr<thread_pool::ThreadPool>& mThreadPoolPtr;
};

}  // namespace io_multiplexer