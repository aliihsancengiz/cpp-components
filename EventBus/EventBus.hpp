#pragma once

#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>

namespace event_bus {

struct EventBus
{
    using eventHandler = std::function<void(std::any)>;

    static EventBus& getInstance()
    {
        static EventBus bus;
        return bus;
    }

    template<typename Event, typename Handler>
    void addHandler(Handler handler)
    {
        const auto type_idx = std::type_index(typeid(Event));
        handlerRegister.emplace(type_idx, [func = std::forward<Handler>(handler)](auto value) {
            func(std::any_cast<Event>(value));
        });
    }
    template<typename Event>
    void removeEvent()
    {
        handlerRegister.erase(std::type_index(typeid(Event)));
    }
    template<typename Event>
    void fireEvent(Event ev)
    {
        auto [beginEventIter, endEventIter] =
          handlerRegister.equal_range(std::type_index(typeid(Event)));
        for (; beginEventIter != endEventIter; ++beginEventIter) {
            beginEventIter->second(std::forward<Event>(ev));
        }
    }

  private:
    EventBus() {}
    std::unordered_multimap<std::type_index, eventHandler> handlerRegister;
};

}  // namespace event_bus
