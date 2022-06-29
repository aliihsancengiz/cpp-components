#pragma once
#include "IOContext.hpp"
#include "IOMultiplexer.hpp"
#include "Logger.hpp"

#include <cstdint>
#include <sys/timerfd.h>
#include <unistd.h>

namespace timer {
enum class TimerType
{
    ONESHOOT,
    PERIODIC
};
using namespace io_multiplexer;
struct Timer : io_multiplexer::ioObject
{
    using TimerCallback = std::function<void()>;

    Timer(io_context::IOContext& io, uint64_t _period, TimerType _type, TimerCallback _cb);
    ~Timer();
    void start();
    void stop();

  private:
    void _timerhandler(ioObject obj, EventType type);
    std::shared_ptr<spdlog::logger> mLogger;
    io_context::IOContext& _io;
    std::uint64_t period;
    TimerType type;
    struct itimerspec new_value;
    TimerCallback cb;
    bool stopped;
};

}  // namespace timer
