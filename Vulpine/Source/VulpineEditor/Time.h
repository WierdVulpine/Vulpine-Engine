#pragma once
#include "Timer.h"

class Time
{
private:
	static inline CommonUtilities::Timer myTimer;

public:
	static float GetDeltaTime() { return myTimer.GetDeltaTime(); }
	static double GetTotalTime() { return myTimer.GetTotalTime(); }
	static void Update() { myTimer.Update(); }
	static void Reset() { myTimer = CommonUtilities::Timer(); }
};

