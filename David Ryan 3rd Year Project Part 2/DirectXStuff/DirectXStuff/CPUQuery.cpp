
// CPUQuery.cpp

#include "CPUQuery.h"


CPUQuery::CPUQuery()
{
}


CPUQuery::~CPUQuery()
{
}

//init sets up query handling
//if it can't get it will set to 0%
void CPUQuery::Init()
{
	PDH_STATUS status;


	// Initialize flag indicating whether object can read system cpu usage or not.
	_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &_queryHandle);
	if(status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}

	// Set query object to poll all cpus in system.
	status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
	if(status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}

	_lastSampleTime = GetTickCount(); 

	_cpuUsage = 0;

	return;
}


//releases handle used in cpu query. 
void CPUQuery::KillSafely()
{
	if(_canReadCpu)
	{
		PdhCloseQuery(_queryHandle);
	}

	return;
}


//call frame function each frame.
//have to ensure only sample each frame. 

void CPUQuery::Update()
{
	PDH_FMT_COUNTERVALUE value; 

	if(_canReadCpu)
	{
		if((_lastSampleTime + 1000) < GetTickCount())
		{
			_lastSampleTime = GetTickCount(); 

			PdhCollectQueryData(_queryHandle);
        
			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);

			_cpuUsage = value.longValue;
		}
	}

	return;
}


//get cpu percentage
//value of cpu 
//if it can't read just  set to 0. 
int CPUQuery::GetCpuPercentage()
{
	int usage;

	if(_canReadCpu)
	{
		usage = (int)_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}

