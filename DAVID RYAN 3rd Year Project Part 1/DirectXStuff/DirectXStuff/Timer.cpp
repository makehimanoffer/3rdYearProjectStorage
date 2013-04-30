
#include "Timer.h"


Timer::Timer()
{
}



Timer::~Timer()
{
}


//returns a frequency. checks if high frequency.
//use value to determine how many counter clicks. 
//use to calculate frame time

bool Timer::Init()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	if(_frequency == 0)
	{
		return false;
	}

	// Find out how many times frequency counter ticks every millisecond.
	_ticksPerMs = (float)(_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	return true;
}

//frame function called every frame of execution

void Timer::Update()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - _startTime);

	_frameTime = timeDifference / _ticksPerMs;

	_startTime = currentTime;

	return;
}

float Timer::GetTime()
{
	return _frameTime;
}