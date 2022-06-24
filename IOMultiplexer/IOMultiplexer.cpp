#include "IOMultiplexer.hpp"

namespace io_multiplexer {
static std::string IOEventTypeToString(const EventType& type)
{
    switch (type) {
        case EventType::READ:
            return "READ";
            break;

        case EventType::WRITE:
            return "WRITE";
            break;

        case EventType::ERROR:
            return "ERROR";
            break;

        case EventType::EDGETRIGERRED:
            return "EDGETRIGERRED";
            break;
        default:
            return "Unknown Event Type";
            break;
    }
}
IOMultiplexer::IOMultiplexer()
{
    mLogger = logger::LoggerConfigurator::getInstance().getLogger("IOMultiplexer");
    mLogger->trace("Constructor");
    mEpollFd = epoll_create(MAX_EVENTS);
    mPollThread = std::thread(std::bind(&IOMultiplexer::pollForIO, this));
}
IOMultiplexer::~IOMultiplexer()
{
    mLogger->trace("Destructor");
    runFlag = false;
    close(mEpollFd);
    mPollThread.join();
}
IOMultiplexer& IOMultiplexer::getInstance()
{
    static IOMultiplexer ioInstance;
    return ioInstance;
}
void IOMultiplexer::registerEvent(const ioObject& ioObj, const EventType& type, Callback cb)
{
    mLogger->trace("Registering Event Fd : {} , Event Type : {}", ioObj.fd,
                   IOEventTypeToString(type));

    epoll_event ev;
    mEventHandlerMap[ioObj.fd][type] = cb;
    ev.events = static_cast<int>(type);
    ev.data.fd = ioObj.fd;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, ioObj.fd, &ev) == -1) {
        mLogger->critical("Unableto add {} File descriptor epoll list.", ioObj.fd);
        mLogger->critical("Error No {}", std::to_string(errno));
        throw std::runtime_error("Error No : " + std::to_string(errno));
    }
}
void IOMultiplexer::deregisterEvent(const ioObject& ioObj, const EventType& type)
{
    mLogger->trace("Deregistering Event Fd : {} ,Event Type : {}", ioObj.fd,
                   IOEventTypeToString(type));
    epoll_ctl(mEpollFd, EPOLL_CTL_DEL, ioObj.fd, NULL);
    mEventHandlerMap[ioObj.fd].erase(type);
}
void IOMultiplexer::pollForIO()
{
    while (runFlag) {
        epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, 100);
        for (int n = 0; n < nfds; ++n) {
            int fd = events[n].data.fd;
            ioObject io{fd};
            EventType type = static_cast<EventType>(events[n].events);

            if (mEventHandlerMap.find(fd) != mEventHandlerMap.end() &&
                mEventHandlerMap[fd].find(type) != mEventHandlerMap[fd].end() &&
                (mEventHandlerMap[fd][type] != nullptr)) {
                try {
                    mLogger->trace(" Got an Event on File Descriptor : {} , Event  : {} ", fd,
                                   IOEventTypeToString(type));
                    mEventHandlerMap[fd][type](io, type);
                } catch (const std::exception& e) {
                }
            }
        }
    }
}

}  // namespace io_multiplexer
