#pragma once

#include <functional>

#include "FlumenCore/Container/Pool.hpp"

namespace core
{
    static constexpr auto DEFAULT_EVENTS_PER_DELEGATE = 8;

    #define EVENT(Function) Event <std::remove_reference <decltype(*this)>::type> (this, &std::remove_reference <decltype(*this)>::type::Function)

    struct EventBase
    {
        virtual void Invoke() = 0;
    };

    template <typename ObjectType>
    struct Event : public EventBase
    {
        ObjectType *object;

        std::function <void (ObjectType *)> function;

        Event(ObjectType *newObject, std::function <void (ObjectType *)> newFunction) : EventBase{}, object(newObject), function(newFunction) {}

        void Invoke() override {function(object);}
    };

    class Delegate
    {
        container::Pool <const EventBase *> events {DEFAULT_EVENTS_PER_DELEGATE};

    public:
        template <typename ObjectType>
        void Add(const Event <ObjectType> &event) 
        {
            *events.Add() = &event;
        }

        template <typename ObjectType>
        void Remove(const Event <ObjectType> &event) 
        {
            events.Remove(event);
        }

        void Invoke()
        {
            for(auto &event : events)
            {
                event->Invoke();
            }
        }

        void Clear()
        {
            events.Reset();
        }
    };
}