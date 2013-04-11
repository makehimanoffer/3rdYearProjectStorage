//precise timer 
//measure exact time of execution
//primarily for synchronizing objects require time frame for movement.

// Timer.h

#ifndef _Timer_H_
#define _Timer_H_



// INCLUDES //

#include <windows.h>



// Class name: Timer

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Init();
	void Update();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif

