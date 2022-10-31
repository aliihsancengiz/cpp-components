#include "Signal.hpp"

namespace signal_handler {

Signal::Signal(io_context::IOContext& io) : _io(io)
{
    mLogger = logger::LoggerConfigurator::getInstance().getLogger("Signal");
    mLogger->trace("Signal Constructor");
    sigemptyset(&mask);
}

Signal::~Signal()
{
    _io.getMultiplexer()->deregisterEvent(*this, io_multiplexer::EventType::READ);
    mLogger->trace("Signal Destructor");
}

void Signal::addSignalHandler(signalNo sigNo, signalHandler handler)
{
    if (mHandlerMap.find(sigNo) == mHandlerMap.end()) {
        mLogger->trace("Adding new signal Handler for Sig NO : {}", sigNo);
        sigaddset(&mask, sigNo);
        mHandlerMap[sigNo] = handler;
    }
}

void Signal::startHandling()
{
    static bool startOnce{true};
    if (startOnce) {
        mLogger->trace("Started to handling signals");
        startOnce = false;
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
            mLogger->error("Error occured during signal handling");
        }
        fd = signalfd(-1, &mask, 0);
        _io.getMultiplexer()->registerEvent(
          *this, io_multiplexer::EventType::READ,
          std::bind(&Signal::_signalProcess, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Signal::_signalProcess(io_multiplexer::ioObject ioObj, io_multiplexer::EventType type)
{
    if (type == io_multiplexer::EventType::READ) {
        signal_info info;
        if (::read(fd, &info, sizeof(info)) != sizeof(info)) {
            return;
        }
        mLogger->trace("Caught new signal... SigNo : {}", info.ssi_signo);
        _io.post(mHandlerMap[info.ssi_signo], info);
    }
}

}  // namespace signal_handler
