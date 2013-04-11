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
	~Timer();

	bool Init();
	void Update();

	float GetTime();

private:
	INT64 _frequency;
	float _ticksPerMs;
	INT64 _startTime;
	float _frameTime;
};

#endif

