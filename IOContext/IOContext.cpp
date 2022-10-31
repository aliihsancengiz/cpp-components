#include "IOContext.hpp"

namespace io_context {

IOContext::IOContext(int numberOfThreads) : threadCount(numberOfThreads)
{
    pool = std::make_shared<thread_pool::ThreadPool>(threadCount);
	multiplexerPtr= std::make_shared<io_multiplexer::IOMultiplexer>(pool);
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
    multiplexerPtr->pollForIO();
}

std::shared_ptr<io_multiplexer::IOMultiplexer>& IOContext::getMultiplexer()
{
	return multiplexerPtr;
}

}  // namespace io_context