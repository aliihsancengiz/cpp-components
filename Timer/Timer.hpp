#pragma once
#include <cstdint>
#include <unistd.h>
#include <sys/timerfd.h>
#include "IOMultiplexer.hpp"
#include "Logger.hpp"

namespace timer
{
    enum class TimerType
    {
        ONESHOOT,
        PERIODIC
    };
    using namespace io_multiplexer;
    struct Timer : io_multiplexer::ioObject
    {
        using TimerCallback = std::function<void()>;

        Timer(uint64_t _period, TimerType _type, TimerCallback _cb);
        ~Timer();
        void start();
        void stop();

    private:
        void _timerhandler(ioObject obj, EventType type);
        std::shared_ptr<spdlog::logger> mLogger;
        std::uint64_t period;
        TimerType type;
        struct itimerspec new_value;
        TimerCallback cb;
        bool stopped;
    };

} // namespace timer
