#pragma once
#include "mission.h"
class CTurn :
	public CMission
{
private:
	TSEND m_temp;
	int m_iLeft;
	int m_iLine;
public:

	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step0(_us (*img)[256]);
	TSEND Bottom(_us (*img)[256]);
	TSEND Step1(_us (*img)[256]);	
	TSEND Step2(_us (*img)[256]);
	TSEND Step3(_us (*img)[256]);
	TSEND LineChk(_us (*img)[256]);
	void LineChk2(_us (*img)[256]);
public:
	CTurn(void);
	~CTurn(void);
};

