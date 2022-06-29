#include "Timer.hpp"

namespace timer {
Timer::Timer(io_context::IOContext& io, uint64_t _period, TimerType _type, TimerCallback _cb)
    : _io(io), period(_period), type(_type), cb(_cb)
{
    fd = timerfd_create(CLOCK_REALTIME, 0);
    mLogger = logger::LoggerConfigurator::getInstance().getLogger("Timer-" + std::to_string(fd));
    mLogger->trace("Timer Constructor");
    mLogger->trace("Create a {} Timer with period of {}",
                   type == TimerType::PERIODIC ? "Periodic" : "Oneshot", period);
}
Timer::~Timer()
{
    mLogger->trace("Timer Destructor");
    close(fd);
}
void Timer::start()
{
    mLogger->trace("Starting timer. Timer File Descriptor {}", fd);
    stopped = false;
    new_value.it_value.tv_sec = period / 1000;
    new_value.it_value.tv_nsec = (period % 1000) * 1000000;
    if (type == TimerType::PERIODIC) {
        new_value.it_interval.tv_sec = period / 1000;
        new_value.it_interval.tv_nsec = (period % 1000) * 1000000;
    } else {
        new_value.it_interval.tv_sec = 0;
        new_value.it_interval.tv_nsec = 0;
    }
    timerfd_settime(fd, 0, &new_value, NULL);
    io_multiplexer::IOMultiplexer::getInstance().registerEvent(
      *this, EventType::READ,
      std::bind(&Timer::_timerhandler, this, std::placeholders::_1, std::placeholders::_2));
}
void Timer::stop()
{
    mLogger->trace("Stopping timer. Timer File Descriptor {}", fd);
    io_multiplexer::IOMultiplexer::getInstance().deregisterEvent(*this, EventType::READ);
    stopped = true;
}
void Timer::_timerhandler(ioObject obj, EventType type)
{
    uint64_t exp;
    if (type == EventType::READ) {
        if (read(fd, &exp, sizeof(exp)) != sizeof(uint64_t)) {
            return;
        }
        if (!stopped) {
            mLogger->trace("Got Timer expiration on File Descriptor {}", fd);
            _io.post(cb);
        }
    }
}
}  // namespace timer
