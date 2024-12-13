#pragma once

#include "../Conventions.hpp"

class Event;

class Delegate
{
	Pool <Event> events;

public:
	Delegate();

	void Invoke() const;

	Delegate& operator += (Event);

	Delegate& operator += (void (*)());

	Delegate& operator -= (Event);

	Delegate& operator -= (void (*)());

	void Clear();
};