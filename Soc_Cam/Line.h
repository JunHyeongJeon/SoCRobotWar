#pragma once
#include "mission.h"





class CLine :
	public CMission
{
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);

	float GetDistance(_us (*img)[256]);
	float GetGradient(_us (*img)[256]);
	
	MV_STEP GetDistanceStep(float fValue);
	MV_STEP GetGradientStep(float fValue);

	bool Gradient(_us (*img)[256],TSEND &tsend);
	bool Distance(_us (*img)[256],TSEND &tsend);

	std::vector<LINE> GetPoint(_us (*img)[256]);
	void doStandardiZation(std::vector<LINE>& vecLine,int nPointCount);

public:
	TSEND Step1(_us (*img)[256]);
public:
	CLine(void);
	~CLine(void);
};
