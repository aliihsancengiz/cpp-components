#pragma once

#include <any>
#include <functional>
#include <unordered_map>
#include <typeindex>

namespace event_bus
{

    struct EventBus
    {
        using eventHandler = std::function<void(std::any)>;

        template <typename Event, typename Handler>
        void addHandler(Handler handler);
        template <typename Event>
        void removeEvent();
        template <typename Event>
        void fireEvent(Event ev);

    private:
        std::unordered_multimap<std::type_index, eventHandler> handlerRegister;
    };
} // namespace event_bus
