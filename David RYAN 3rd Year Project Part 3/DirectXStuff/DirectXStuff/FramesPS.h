
 

#ifndef _FramesPS_H_
#define _FramesPS_H_



// LINKING //

#pragma comment(lib, "winmm.lib")



// INCLUDES //

#include <windows.h>
#include <mmsystem.h>






//counter with a timer associated with it
class FramesPS
{
public:
	FramesPS();
	FramesPS(const FramesPS&);
	~FramesPS();

	void Init();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif