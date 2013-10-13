#include "CTimeManager.h"

CTimeManager::CTimeManager()
{
	
}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::Init()
{
	clock_gettime(CLOCK_REALTIME,&sCurrentTime);
}

double CTimeManager::Process()
{
	sPrevTime = sCurrentTime;
	clock_gettime(CLOCK_REALTIME,&sCurrentTime);
	dCurrentTime = (double)sCurrentTime.tv_sec + (double)sCurrentTime.tv_nsec / (double)1000000000;
	dPrevTime = (double)sPrevTime.tv_sec + (double)sPrevTime.tv_nsec / (double)1000000000;
	return dCurrentTime - dPrevTime;
}
