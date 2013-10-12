#pragma once
#include "mission.h"
class CGolf :
	public CMission
{
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step1(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
public:
	CGolf(void);
	~CGolf(void);
};

