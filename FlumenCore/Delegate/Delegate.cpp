#include "Delegate.hpp"
#include "Event.hpp"

Delegate::Delegate()
{
	events.Initialize(8);
}

void Delegate::Invoke() const
{
	for(auto &event : events)
	{
		event.Invoke();
	}
}

Delegate& Delegate::operator += (Event _event)
{
	auto event = events.Add();

	*event = _event;

	return *this;
}

Delegate& Delegate::operator += (void (*callback)())
{
	auto event = events.Add();

	*event = callback;

	return *this;
}

Delegate& Delegate::operator -= (Event _event)
{
	for(auto &event : events)
	{
		if(event == _event)
		{
			events.RemoveAt(&event);
			break;
		}
	}

	return *this;
}

Delegate& Delegate::operator -= (void (*callback)())
{
	for(auto &event : events)
	{
		if(event == callback)
		{
			events.RemoveAt(&event);
			break;
		}
	}

	return *this;
}

void Delegate::Clear()
{
	events.Reset();
}