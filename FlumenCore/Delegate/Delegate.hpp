#pragma once

#include "../Conventions.hpp"

class Delegate
{
	Pool <Callback> callbacks_;
public:
	Delegate();

	void Invoke();

	template <class ClassType, class ObjectType>
	void Add(ObjectType*, void (ClassType::*)());

	void Add(void (*)());

	template <class ClassType, class ObjectType>
	void Remove(ObjectType*, void (ClassType::*)());

	void Clear();
};

template <class ClassType, class ObjectType>
void Delegate::Add(ObjectType *object, void (ClassType::*function)())
{
	auto callback = callbacks_.Add();
	if(callback == nullptr)
		return;

	callback->bind(object, function);
}

template <class ClassType, class ObjectType>
void Delegate::Remove(ObjectType *object, void (ClassType::*function)())
{
	Callback externalCallback(object, function);

	Callback* targetCallback = nullptr;
	for(auto callback = callbacks_.GetStart(); callback != callbacks_.GetEnd(); ++callback)
	{
		if(*callback == externalCallback)
		{
			targetCallback = callback;
			callback->clear();
			break;
		}
	}

	if(targetCallback != nullptr)
	{
		callbacks_.Remove(targetCallback);
	}
}


