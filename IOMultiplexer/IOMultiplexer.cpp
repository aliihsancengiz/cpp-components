#include "IOMultiplexer.hpp"

namespace io_multiplexer
{
    IOMultiplexer::IOMultiplexer()
    {
        mEpollFd = epoll_create(MAX_EVENTS);
        mPollThread = std::thread(std::bind(&IOMultiplexer::pollForIO, this));
    }
    IOMultiplexer::~IOMultiplexer()
    {
        runFlag = false;
        close(mEpollFd);
        mPollThread.join();
    }
    IOMultiplexer &IOMultiplexer::getInstance()
    {
        static IOMultiplexer ioInstance;
        return ioInstance;
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
        epoll_ctl(mEpollFd, EPOLL_CTL_DEL, ioObj.fd, NULL);
        mEventHandlerMap[ioObj.fd].erase(type);
    }
    void IOMultiplexer::pollForIO()
    {
        while (runFlag)
        {
            epoll_event events[MAX_EVENTS];
            int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, 100);
            for (int n = 0; n < nfds; ++n)
            {
                int fd = events[n].data.fd;
                ioObject io{fd};
                EventType type = static_cast<EventType>(events[n].events);

                if (mEventHandlerMap.find(fd) != mEventHandlerMap.end() && mEventHandlerMap[fd].find(type) != mEventHandlerMap[fd].end() && (mEventHandlerMap[fd][type] != nullptr))
                {
                    try
                    {
                        mEventHandlerMap[fd][type](io, type);
                    }
                    catch (const std::exception &e)
                    {
                    }
                }
            }
        }
    }

} // namespace io_multiplexer
