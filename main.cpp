#include "IOMultiplexer.hpp"
#include "EventBus.hpp"

void readHandler(io_multiplexer::ioObject obj, io_multiplexer::EventType t)
{
	std::cout << "There someting to be read" << std::endl;
	std::string ss;
	std::cin >> ss;
	std::cout << "Readed " << ss << std::endl;
}

int main()
{
	io_multiplexer::IOMultiplexer mult;
	io_multiplexer::ioObject obj{0};

	mult.registerEvent(obj, io_multiplexer::EventType::READ, readHandler);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	mult.deregisterEvent(obj, io_multiplexer::EventType::READ);
	return 0;
}
