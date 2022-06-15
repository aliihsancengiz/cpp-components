#include "IOMultiplexer.hpp"
#include "EventBus.hpp"
#include "Timer.hpp"
#include <mutex>

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
int main()
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

	return 0;
}
