#pragma once
#include "mission.h"
class CHurdle :
	public CMission
{
private:
	int m_iSee;
	int m_iChk;
	int m_please;
	int m_kkk;
	bool bkkk;
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step3(_us (*img)[256]);
	TSEND Step1(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
	TSEND Finish(_us (*img)[256]);
public:
	CHurdle(void);
	~CHurdle(void);
};
