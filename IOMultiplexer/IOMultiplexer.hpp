#pragma once

#include "Logger.hpp"

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
    std::int32_t fd;
};

enum class EventType : int64_t
{
    READ = EPOLLIN,
    WRITE = EPOLLOUT,
    ERROR = EPOLLERR,
    EDGETRIGERRED = EPOLLET
};

using Callback = std::function<void(ioObject, EventType)>;

struct IOMultiplexer
{

    using EventHandlerMap = std::map<std::int32_t, std::map<EventType, Callback>>;
    typedef struct epoll_event epool_event;
    constexpr static int MAX_EVENTS = 255;

    IOMultiplexer(const IOMultiplexer&) = delete;
    IOMultiplexer& operator=(const IOMultiplexer&) = delete;
    ~IOMultiplexer();

    static IOMultiplexer& getInstance();

    void registerEvent(const ioObject& ioObj, const EventType& type, Callback cb);
    void deregisterEvent(const ioObject& ioObj, const EventType& type);

    void pollForIO();

  private:
    EventHandlerMap mEventHandlerMap;
    std::int32_t mEpollFd;
    std::shared_ptr<spdlog::logger> mLogger;

    IOMultiplexer();
};

}  // namespace io_multiplexer