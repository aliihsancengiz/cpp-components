#include "Event.hpp"
#include "EventBus.hpp"
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

void MyDataHandler(MyData data);
void MyDataHandler2(MyData data);

void TimerUseCae();
void EventUseCase();

bool stopFlag = false;

void handle_signal(struct signalfd_siginfo info)
{
    std::cout << "Got SIGINT signal No " << info.ssi_signo << std::endl;
    stopFlag = true;
}
void handle_signal2(struct signalfd_siginfo info)
{
    std::cout << "Got SIGTERM signal No" << info.ssi_signo << std::endl;
    stopFlag = true;
}

void threadpollUseCase()
{
    thread_pool::ThreadPool pool(3);
    pool.dispatch([](auto id) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "I am task  1 " << std::endl;
    });
    pool.dispatch([](auto id) {
        std::cout << "I am task 2 " << std::endl;
    });
    pool.dispatch([](auto id) {
        std::cout << "I am task 3 " << std::endl;
    });
    pool.dispatch([](auto id) {
        std::cout << "I am task 4 " << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
int main()
{

    logger::LoggerConfigurator::getInstance().setLogLevel(logger::LogLevel::TRACE);
    logger::LoggerConfigurator::getInstance().setLogTarget(logger::LogTarget::CONSOLE);

    // TimerUseCae();
    // EventUseCase();
    // threadpollUseCase();
    signal_handler::Signal ss;
    ss.addSignalHandler(SIGINT, handle_signal);
    ss.addSignalHandler(SIGTERM, handle_signal2);

    ss.startHandling();
    while (!stopFlag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

void foo(int k)
{
    std::cout << "Free Function Callback " << k << std::endl;
}

struct Foo
{
    Foo()
    {
        y = 100;
    }
    void operator()()
    {
        std::cout << "Functor Object Callback " << y << std::endl;
    }

  private:
    int y;
};

void TimerUseCae()
{
    timer::Timer t(1000, timer::TimerType::PERIODIC, std::bind(&foo, 1000));
    timer::Timer t2(500, timer::TimerType::PERIODIC, []() {
        std::cout << "Lambda Callback" << std::endl;
    });
    timer::Timer t3(500, timer::TimerType::PERIODIC, std::bind(Foo()));

    t.start();
    t2.start();
    t3.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    t.stop();
    t2.stop();
    t3.stop();
}
void EventUseCase()
{
    event::Event<MyData> mMydataHandler;
    mMydataHandler.addSubscriber(MyDataHandler);
    mMydataHandler.addSubscriber(MyDataHandler2);

    for (int i = 0; i < 2; i++) {
        mMydataHandler.sendEvent(MyData{i});
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void MyDataHandler(MyData data)
{
    std::cout << "MyDataHandler : " << data.foo << std::endl;
}
void MyDataHandler2(MyData data)
{
    std::cout << "MyDataHandler2 : " << data.foo << std::endl;
}