#include "IOMultiplexer.hpp"
#include "EventBus.hpp"
#include "Timer.hpp"
#include <mutex>
#include "Event.hpp"

struct MyData
{
	int foo;
};

void MyDataHandler(MyData data)
{
	std::cout << "MyDataHandler : " << data.foo << std::endl;
}
void MyDataHandler2(MyData data)
{
	std::cout << "MyDataHandler2 : " << data.foo << std::endl;
}
int main()
{
	event::Event<MyData> mMydataHandler;
	mMydataHandler.addSubscriber(MyDataHandler);
	mMydataHandler.addSubscriber(MyDataHandler2);

	for (int i = 0; i < 1000; i++)
	{
		mMydataHandler.sendEvent(MyData{i});
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
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
	timer::Timer t2(500, timer::TimerType::PERIODIC, []()
					{ std::cout << "Lambda Callback" << std::endl; });
	timer::Timer t3(500, timer::TimerType::PERIODIC, std::bind(Foo()));

	t.start();
	t2.start();
	t3.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(1200));

	t.stop();
	t2.stop();
	t3.stop();
}
