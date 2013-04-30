
#include "FramesPS.h"


FramesPS::FramesPS()
{
}




FramesPS::~FramesPS()
{
}

//Init function sets all counters to zero and starts timer.

void FramesPS::Init()
{
	_fps = 0;
	_count = 0;
	_startTime = timeGetTime();
	return;
}

//frame must be called each frame.
//so it can increment by 1. 
//if one second has elapsed.
void FramesPS::Update()
{
	_count++;

	if(timeGetTime() >= (_startTime + 1000))
	{
		_fps = _count;
		_count = 0;
		
		_startTime = timeGetTime();
	}
}


int FramesPS::GetFps()
{
	return _fps;
}



