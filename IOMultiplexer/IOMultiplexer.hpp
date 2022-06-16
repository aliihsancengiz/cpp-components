#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>
#include "Logger.hpp"

namespace io_multiplexer
{
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

        IOMultiplexer(const IOMultiplexer &) = delete;
        IOMultiplexer &operator=(const IOMultiplexer &) = delete;
        ~IOMultiplexer();

        static IOMultiplexer &getInstance();

        void registerEvent(const ioObject &ioObj, const EventType &type, Callback cb);
        void deregisterEvent(const ioObject &ioObj, const EventType &type);

    private:
        EventHandlerMap mEventHandlerMap;
        std::int32_t mEpollFd;
        std::thread mPollThread;
        bool runFlag{true};
        std::shared_ptr<spdlog::logger> mLogger;
        void pollForIO();

        IOMultiplexer();
    };

}