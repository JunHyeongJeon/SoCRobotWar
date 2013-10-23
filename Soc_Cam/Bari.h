#pragma once
#include "Mission.h"
class CBari	: public CMission
{
private:
	bool m_ifuck;
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step1(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
	TSEND Step3(_us (*img)[256]);
	TSEND Finish(_us (*img)[256]);
public:
	CBari(void);
	~CBari(void);
};
