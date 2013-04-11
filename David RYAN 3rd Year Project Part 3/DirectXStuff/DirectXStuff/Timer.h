//precise timer 
//measure exact time of execution
//primarily for synchronizing objects require time frame for movement.

 

#ifndef _Timer_H_
#define _Timer_H_



// INCLUDES //

#include <windows.h>





class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Init();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif

