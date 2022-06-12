#include "IOMultiplexer.hpp"

namespace io_multiplexer
{
    IOMultiplexer::IOMultiplexer()
    {
        mEpollFd = epoll_create(MAX_EVENTS);
        mPollThread = std::thread(std::bind(&IOMultiplexer::pollForIO, this));
    }
    IOMultiplexer &IOMultiplexer::getInstance()
    {
        static IOMultiplexer ioInstance;
        return ioInstance;
    }
    IOMultiplexer::~IOMultiplexer()
    {
        mPollThread.join();
    }
    void IOMultiplexer::registerEvent(const ioObject &ioObj, const EventType &type, Callback cb)
    {
        epoll_event ev;
        mEventHandlerMap[ioObj.fd][type] = cb;
        ev.events = static_cast<int>(type);
        ev.data.fd = ioObj.fd;
        if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, ioObj.fd, &ev) == -1)
        {
            throw std::runtime_error("Error No : " + std::to_string(errno));
        }
    }
    void IOMultiplexer::deregisterEvent(const ioObject &ioObj, const EventType &type)
    {
        mEventHandlerMap[ioObj.fd].erase(type);
        epoll_ctl(mEpollFd, EPOLL_CTL_DEL, ioObj.fd, NULL);
    }
    void IOMultiplexer::pollForIO()
    {
        while (true)
        {
            epoll_event events[5];
            int nfds = epoll_wait(mEpollFd, events, 5, -1);
            for (int n = 0; n < nfds; ++n)
            {
                int fd = events[n].data.fd;
                ioObject io{fd};
                EventType type = static_cast<EventType>(events[n].events);
                mEventHandlerMap[fd][type](io, type);
            }
        }
    }

} // namespace io_multiplexer
