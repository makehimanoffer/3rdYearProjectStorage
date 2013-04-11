
 

#include "Timer.h"


Timer::Timer()
{
}


Timer::Timer(const Timer& other)
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
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

//frame function called every frame of execution

void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float Timer::GetTime()
{
	return m_frameTime;
}