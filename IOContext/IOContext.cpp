#include "IOContext.hpp"

namespace io_context {

IOContext::IOContext(int numberOfThreads) : threadCount(numberOfThreads)
{
    pool = std::make_unique<thread_pool::ThreadPool>(threadCount);
    mLogger = logger::LoggerConfigurator::getInstance().getLogger("IOContext");
    mLogger->trace("Constructor");
    mLogger->trace("A Thread Pool of {} thread is started", threadCount);
}

IOContext::~IOContext()
{
    mLogger->trace("Destructor");
}

void IOContext::post(thread_pool::Work&& w)
{
    mLogger->trace("New Work Posted to Thread pool");
    pool->dispatch(std::forward<thread_pool::Work>(w));
}
void IOContext::run()
{
    io_multiplexer::IOMultiplexer::getInstance().pollForIO();
}

}  // namespace io_context