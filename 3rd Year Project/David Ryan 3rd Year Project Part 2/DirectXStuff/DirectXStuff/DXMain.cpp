
// main.cpp

#include "SystemBackend.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemBackend* System;
	bool result;

	System = new SystemBackend;
	if(!System)
	{
		return 0;
	}

	// Initialize and run system object.
	result = System->Init();
	if(result)
	{
		System->Run();
	}

	// Shutdown and release system object.
	System->KillSafely();
	delete System;
	System = 0;

	return 0;
}