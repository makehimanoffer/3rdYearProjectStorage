
#include "SystemBackend.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemBackend* System;
	bool result;
	
	
	// Create the system object.
	System = new SystemBackend;
	if(!System)
	{
		return 0;
	}

	// Init and run the system object.
	result = System->Init();
	if(result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->KillSafely();
	delete System;
	System = 0;

	return 0;
}