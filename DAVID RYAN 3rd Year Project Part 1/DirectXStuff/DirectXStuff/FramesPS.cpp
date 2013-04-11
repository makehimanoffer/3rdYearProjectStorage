
#include "FramesPS.h"


FramesPS::FramesPS()
{
}


FramesPS::FramesPS(const FramesPS& other)
{
}


FramesPS::~FramesPS()
{
}

//The Init function sets all the counters to zero and starts the timer.

void FramesPS::Init()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

//frame must be called each frame.
//so it can increment by 1. 
//if one second has elapsed.
void FramesPS::Update()
{
	m_count++;

	if(timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		
		m_startTime = timeGetTime();
	}
}


int FramesPS::GetFps()
{
	return m_fps;
}



