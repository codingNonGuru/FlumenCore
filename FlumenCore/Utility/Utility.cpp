#include "FlumenCore/Utility/Utility.hpp"

#include "FlumenCore/Conventions.hpp"

std::random_device randomDevice;
std::mt19937 twister(randomDevice());

namespace utility
{
	float GetRandom(float minimum, float maximum)
	{
		std::uniform_real_distribution<float> distribution(minimum, maximum);
		return distribution(twister);
	}

	int GetRandom(int minimum, int maximum)
	{
		std::uniform_int_distribution<int> distribution(minimum, maximum);
		return distribution(twister);
	}

	float GetRandom(float minValue, float maxValue, float expectedValue, float standardDeviation)
	{
		float value = GetRandom(minValue, maxValue);
		float gaussChance = 1.0f - exp(-pow(value - expectedValue, 2.0f) / (2.0f * pow(standardDeviation, 2.0f)));

		if(RollDice<float>(gaussChance) == true)
			return GetRandom(minValue, maxValue, expectedValue, standardDeviation);
		else
			return value;
	}

	float Interpolate(float t)
	{
		return 3.0f * t * t - 2.0f * t * t * t;
	}

	float Clamp(float value, float minimum, float maximum)
	{
		if(value > maximum)
		{
			return maximum;
		}
		else if(value < minimum)
		{
			return minimum;
		}

		return value;
	}

	void Clamp(int &value, int minimum, int maximum)
	{
		if(value > maximum)
		{
			value = maximum;
		}
		else if(value < minimum)
		{
			value = minimum;
		}
	}

	float ComputeSign(Float2& p1, Float2& p2, Float2& p3)
	{
	    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	bool IsInsideTriangle (Float2& point, Float2& v1, Float2& v2, Float2& v3)
	{
	    bool b1, b2, b3;

	    b1 = ComputeSign(point, v1, v2) < 0.0f;
	    b2 = ComputeSign(point, v2, v3) < 0.0f;
	    b3 = ComputeSign(point, v3, v1) < 0.0f;

	    return ((b1 == b2) && (b2 == b3));
	}
}
