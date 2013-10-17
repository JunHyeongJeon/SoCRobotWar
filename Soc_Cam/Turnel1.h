#pragma once
#include "mission.h"
class CTurnel1 :
	public CMission
{
private:
	int m_iLeft;
	int m_iRight;	
	int iDir;
	int m_iLine;
	bool m_bLeft;
	bool m_bRight;
	TSEND m_temp;
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	int CheckMid(_us (*img)[256]);
	TSEND Step1(_us (*img)[256]);
	TSEND LineChk(_us (*img)[256]);
	void LineChk2(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
public:
	CTurnel1(void);
	~CTurnel1(void);
};
