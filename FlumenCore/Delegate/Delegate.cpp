#include "Delegate.hpp"

Delegate::Delegate()
{
	callbacks_.Initialize(4);
}

void Delegate::Invoke()
{
	for(auto callback = callbacks_.GetStart(); callback != callbacks_.GetEnd(); ++callback)
	{
		if(callback->empty())
			continue;

		(*callback)();
	}
}

void Delegate::Add(void (*function)())
{
	auto callback = callbacks_.Add();
	if(callback == nullptr)
		return;

	callback->bind(function);
}

void Delegate::Clear()
{
	for(auto callback = callbacks_.GetStart(); callback != callbacks_.GetEnd(); ++callback)
	{
		if(callback->empty())
			continue;

		callback->clear();
	}
}



