///////////////////////////////////////////////////////////////////////////////
// Filename: CPUQuery.cpp
///////////////////////////////////////////////////////////////////////////////
#include "CPUQuery.h"


CPUQuery::CPUQuery()
{
}


CPUQuery::CPUQuery(const CPUQuery& other)
{
}


CPUQuery::~CPUQuery()
{
}

//init sets up query handling
//if it can't get it will set to 0%
void CPUQuery::Initialize()
{
	PDH_STATUS status;


	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount(); 

	m_cpuUsage = 0;

	return;
}


//releases handle used in cpu query. 
void CPUQuery::Shutdown()
{
	if(m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}

	return;
}


//call frame function each frame.
//have to ensure we only sample each frame. 

void CPUQuery::Frame()
{
	PDH_FMT_COUNTERVALUE value; 

	if(m_canReadCpu)
	{
		if((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount(); 

			PdhCollectQueryData(m_queryHandle);
        
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
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

	if(m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}

