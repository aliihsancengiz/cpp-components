#include "EventBus.hpp"

namespace event_bus
{
    template <typename Event, typename Handler>
    void EventBus::addHandler(Handler handler)
    {
        const auto type_idx = std::type_index(typeid(Event));
        handlerRegister.emplace(type_idx, [func = std::forward<Handler>(handler)](auto value)
                                { func(std::any_cast<Event>(value)); });
    }
    template <typename Event>
    void EventBus::removeEvent()
    {
        handlerRegister.erase(std::type_index(typeid(Event)));
    }
    template <typename Event>
    void EventBus::fireEvent(Event ev)
    {
        auto [beginEventIter, endEventIter] =
            handlerRegister.equal_range(std::type_index(typeid(Event)));
        for (; beginEventIter != endEventIter; ++beginEventIter)
        {
            beginEventIter->second(std::forward<Event>(ev));
        }
    }

} // namespace event_bus
