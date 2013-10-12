#pragma once
#include "mission.h"
class CTurnel1 :
	public CMission
{
private:
	_us (*img_left)[256];
	_us (*img_right)[256];
	int iDir;
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step1(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
	TSEND Step3(_us (*img)[256]);
	TSEND Step4(_us (*img)[256]);
public:
	CTurnel1(void);
	~CTurnel1(void);
};
