#pragma once
#include <time.h>

class CTimeManager
{
public:
	CTimeManager();
	~CTimeManager();
	double Process();
	void Init();

private:

	struct timespec sCurrentTime;
	struct timespec sPrevTime;
	double dCurrentTime;
	double dPrevTime;
};
