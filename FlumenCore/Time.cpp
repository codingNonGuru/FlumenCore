#include <chrono>
#include <iostream>

#include "Time.hpp"

float Time::delta_ = 0.0f;

double Time::lastTimeStamp_ = 0.0;

double Time::currentTimeStamp_ = 0.0;

double Time::clockStart_ = 0.0;

void Time::Update()
{
	using namespace std::chrono;
	milliseconds timeStamp = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	lastTimeStamp_ = currentTimeStamp_;

	currentTimeStamp_ = timeStamp.count() / 1000.0;

	delta_ = (float)(currentTimeStamp_ - lastTimeStamp_);
}

float Time::GetDelta()
{
	return delta_;
}

void Time::StartClock()
{
	using namespace std::chrono;
	auto timeStamp = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());

	clockStart_ = timeStamp.count();
}

float Time::GetClock(bool isReset)
{
	using namespace std::chrono;
	auto timeStamp = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());

	auto delta = timeStamp.count() - clockStart_;

	if(isReset)
	{
		clockStart_ = timeStamp.count();
	}

	return (float)delta / 1000.0f;
}
