
#ifndef _CPUQuery_H_
#define _CPUQuery_H_

//pdh include to query cpu usage

#pragma comment(lib, "pdh.lib")

#include <pdh.h>


class CPUQuery
{
public:
	CPUQuery();
	CPUQuery(const CPUQuery&);
	~CPUQuery();

	void Init();
	void KillSafely();
	void Update();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif


