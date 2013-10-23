#pragma once
#include "mission.h"

class CLine :
	public CMission
{
private:
	bool LRepeat;
	bool RRepeat;
	int LChk;
	int RChk;
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);

public:
	TSEND Step1(_us (*img)[256]);
public:
	CLine(void);
	~CLine(void);
};
