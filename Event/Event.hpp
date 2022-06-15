#pragma once
#include "IOMultiplexer.hpp"
#include <queue>
#include <sys/eventfd.h>

namespace event
{
    template <typename TData>
    struct Event : io_multiplexer::ioObject
    {
        using handler = std::function<void(TData data)>;
        Event()
        {
            fd = ::eventfd(0, 0);
            io_multiplexer::IOMultiplexer::getInstance().registerEvent(io_multiplexer::ioObject{fd}, io_multiplexer::EventType::READ, std::bind(&Event<TData>::readHandler, this, std::placeholders::_1, std::placeholders::_2));
        }
        ~Event()
        {
            std::cout << mEventQueue.size() << std::endl;
            io_multiplexer::IOMultiplexer::getInstance().deregisterEvent(io_multiplexer::ioObject{fd}, io_multiplexer::EventType::READ);
        }
        void sendEvent(TData data)
        {
            mEventQueue.emplace(data);
            if (::write(fd, &counter, sizeof(uint64_t)) == sizeof(counter))
            {
                counter++;
            }
            else
            {
                std::cout << "Failed to write" << std::endl;
            }
        }
        void addSubscriber(handler h)
        {
            mHandlerList.emplace_back(h);
        }

    private:
        uint64_t counter{1};
        std::vector<handler> mHandlerList;
        std::queue<TData> mEventQueue;
        void readHandler(io_multiplexer::ioObject io, io_multiplexer::EventType type)
        {
            if (type == io_multiplexer::EventType::READ)
            {
                uint64_t counter;
                ::read(fd, &counter, sizeof(counter));
                while (!mEventQueue.empty())
                {
                    TData data = mEventQueue.front();
                    mEventQueue.pop();
                    for (auto observer : mHandlerList)
                    {
                        if (observer != nullptr)
                        {
                            observer(data);
                        }
                    }
                }
            }
        }
    };

} // namespace event
