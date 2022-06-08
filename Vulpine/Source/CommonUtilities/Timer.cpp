#include "pch.h"
#include "Timer.h"

namespace CommonUtilities 
{
	Timer::Timer() : myDeltaTime(0), myTotalTime(0)
	{
		myPreviousTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::Update()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsed = currentTime - myPreviousTime;

		myDeltaTime = elapsed;
		myTotalTime += elapsed;

		myPreviousTime = currentTime;
	}

    float Timer::GetDeltaTime() const
	{
		return myDeltaTime.count();
	}

    double Timer::GetTotalTime() const
	{
		return myTotalTime.count();
	}
}