#include "Event.hpp"
#include "EventBus.hpp"
#include "IOContext.hpp"
#include "IOMultiplexer.hpp"
#include "Logger.hpp"
#include "Signal.hpp"
#include "Socket.hpp"
#include "ThreadPool.hpp"
#include "Timer.hpp"

#include <mutex>

struct MyData
{
    int foo;
};

int main()
{

    logger::LoggerConfigurator::getInstance().setLogLevel(logger::LogLevel::TRACE);
    logger::LoggerConfigurator::getInstance().setLogTarget(logger::LogTarget::CONSOLE);

    io_context::IOContext context;

    ip::tcp::Acceptor acp(context, ip::Endpoint("127.0.0.1", 4444));

    acp.async_accept([](ip::Socket sock) {
        std::cout << "Got new Connection " << sock.fd << std::endl;
        sock.close();
    });

    while (true) {
        context.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
