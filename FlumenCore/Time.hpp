#pragma once

class Time
{
private:
	static float delta_;

	static double lastTimeStamp_;

	static double currentTimeStamp_;

	static double clockStart_;

public:
	static void Update();

	static float GetDelta();

	static void StartClock();

	static float GetClock(bool = true);
};
