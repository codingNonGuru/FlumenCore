#include "Delegate.hpp"

Delegate::Delegate()
{
	callbacks_.Initialize(8);
}

void Delegate::Invoke()
{
	callbacks_.Do([](auto &callback) -> bool
	{
		callback();
	});
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
	callbacks_.Reset();
}