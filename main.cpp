#include "Event.hpp"
#include "EventBus.hpp"
#include "IOContext.hpp"
#include "IOMultiplexer.hpp"
#include "Logger.hpp"
#include "Signal.hpp"
#include "ThreadPool.hpp"
#include "Timer.hpp"

#include <mutex>

struct MyData
{
    int foo;
};

std::atomic<bool> stopped{false};

void signalHandler(int signum)
{
    stopped = true;
}

int main()
{
    logger::LoggerConfigurator::getInstance().setLogLevel(logger::LogLevel::TRACE);
    logger::LoggerConfigurator::getInstance().setLogTarget(logger::LogTarget::CONSOLE);

    io_context::IOContext context;

    signal(SIGINT, signalHandler);

    event::Event<MyData> mMydataHandler(context);
    mMydataHandler.addSubscriber([](MyData data) {
        std::cout << "Got Event" << std::endl;
    });

    timer::Timer t2(context, 1000, timer::TimerType::PERIODIC, [&mMydataHandler]() {
        std::cout << "Lambda Callback" << std::endl;
        mMydataHandler.sendEvent(MyData{12});
    });

    t2.start();

    while (!stopped.load()) {
        context.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
