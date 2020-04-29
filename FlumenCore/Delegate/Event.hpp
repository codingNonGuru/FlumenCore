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

    template <class ClassType, class ObjectType>
	Event(void (*function)())
    {
        callback.bind(function);
    }

    void Invoke()
    {
        callback();
    }
};