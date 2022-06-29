#pragma once
#include "IOContext.hpp"
#include "IOMultiplexer.hpp"
#include "Logger.hpp"

#include <signal.h>
#include <sys/signalfd.h>

typedef struct signalfd_siginfo signal_info;
typedef int signalNo;

namespace signal_handler {

struct Signal : io_multiplexer::ioObject
{
    using signalHandler = std::function<void(signal_info)>;
    Signal(io_context::IOContext& io);
    ~Signal();

    void addSignalHandler(signalNo sigNo, signalHandler handler);
    void startHandling();

  private:
    io_context::IOContext& _io;
    std::map<signalNo, signalHandler> mHandlerMap;
    std::shared_ptr<spdlog::logger> mLogger;
    sigset_t mask;
    void _signalProcess(io_multiplexer::ioObject ioObj, io_multiplexer::EventType type);
};
}  // namespace signal_handler
