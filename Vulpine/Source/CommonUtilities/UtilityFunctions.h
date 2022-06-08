#pragma once
#include <cassert>
#include <random>

namespace CommonUtilities
{
	template<typename T>
	T Max(const T aFirstValue, const T aSecondValue)
	{
		if (aFirstValue > aSecondValue)
		{
			return aFirstValue;
		}
		return aSecondValue;
	}

	template<typename T>
	T Min(const T aFirstValue, const T aSecondValue)
	{
		if (aFirstValue < aSecondValue)
		{
			return aFirstValue;
		}
		return aSecondValue;
	}

	template<typename T>
	T Clamp(const T aValue, const T aMin, const T aMax)
	{
		assert(aMin <= aMax && "ERROR! min value is larger than Max value.");

		if (aValue <= aMin)
		{
			return aMin;
		}
		else if (aValue >= aMax)
		{
			return aMax;
		}
		return aValue;
	}

	template<typename T>
	T Abs(const T aValue)
	{
		if (aValue < 0)
		{
			return aValue * -1;
		}
		return aValue;
	}

	template<typename T>
	void Swap(T& aFirstValue, T& aSecondValue)
	{
		T temp = aFirstValue;
		aFirstValue = aSecondValue;
		aSecondValue = temp;
	}

	template<typename T>
	T Lerp(const T aFirstValue, const T aSecondValue, const float aInterpolationValue)
	{
		return (aFirstValue + (aInterpolationValue * (aSecondValue - aFirstValue)));
	}

	int GenerateRandom(int aMin, int aMax)
	{
		std::random_device device;
		std::mt19937 rng(device());
		std::uniform_int_distribution<std::mt19937::result_type> dist(aMin, aMax);

		return dist(rng);
	}
}

