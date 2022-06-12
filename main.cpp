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
	std::int32_t stdInputFd{0};
	io_multiplexer::ioObject obj{stdInputFd};

	io_multiplexer::IOMultiplexer::getInstance().registerEvent(obj, io_multiplexer::EventType::READ, readHandler);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	io_multiplexer::IOMultiplexer::getInstance().deregisterEvent(obj, io_multiplexer::EventType::READ);
	return 0;
}
