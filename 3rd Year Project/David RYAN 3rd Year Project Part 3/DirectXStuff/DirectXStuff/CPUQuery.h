


#ifndef _CPUQuery_H_
#define _CPUQuery_H_

//pdh include to query cpu usage

// LINKING //

#pragma comment(lib, "pdh.lib")


// INCLUDES //

#include <pdh.h>





class CPUQuery
{
public:
	CPUQuery();
	~CPUQuery();

	void Init();
	void KillSafely();
	void Update();
	int GetCpuPercentage();

private:
	bool _canReadCpu;
	HQUERY _queryHandle;
	HCOUNTER _counterHandle;
	unsigned long _lastSampleTime;
	long _cpuUsage;
};

#endif


