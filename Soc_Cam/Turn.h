#pragma once
#include "c:\cygwin\home\socrobot\soc_cam\mission.h"
class CTurn :
	public CMission
{
public:
	TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step1(_us (*img)[256]);	
public:
	CTurn(void);
	~CTurn(void);
};

