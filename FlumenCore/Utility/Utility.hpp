#pragma once

#include <math.h>
#include <random>

#include <glm/glm.hpp>

typedef glm::vec2 Float2;

namespace utility
{
	float GetRandom(float minimum, float maximum);

	int GetRandom(int min, int max);

	float GetRandom(float, float, float, float);

	template<typename ValueType>
	bool RollDice(ValueType chance)
	{
		return GetRandom(0.0f, 1.0f) < chance;
	}

	float Interpolate(float);

	void Clamp(float &, float = 0.0f, float = 1.0f);

	float ComputeSign(Float2 &, Float2 &, Float2 &);

	bool IsInsideTriangle(Float2 &, Float2 &, Float2 &, Float2 &);
}
