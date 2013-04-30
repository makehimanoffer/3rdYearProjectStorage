
 

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
	~FramesPS();

	void Init();
	void Update();
	int GetFps();

private:
	int _fps, _count;
	unsigned long _startTime;
};

#endif