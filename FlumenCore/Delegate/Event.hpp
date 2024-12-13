#pragma once

#include "FlumenCore/Conventions.hpp"

class Event
{
    Callback callback;

public:
    Event() : callback() {}

    template <class ClassType, class ObjectType>
	Event(ObjectType *object, void (ClassType::*function)())
    {
        callback.bind(object, function);
    }

	Event(void (*function)())
    {
        callback.bind(function);
    }

    bool operator==(const Event &other)
    {
        return (callback == other.callback);
    }

    bool IsValid() const
    {
        return callback.empty() == false;
    }

    void Invoke() const
    {
        callback();
    }

    void Clear()
    {
        callback.clear();
    }
};