#pragma once
#include "mission.h"

struct point
{
	int x;
	int y;

	point(int _x,int _y) 
	{
		x = _x;
		y = _y;
	}

	point()
	{

	}
};

struct LINE
{
	point p1;
	point p2;

	LINE(point _p1,point _p2)
	{
		p1 = _p1;
		p2 = _p2;
	};
};

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

public:
	TSEND Step1(_us (*img)[256]);
public:
	CLine(void);
	~CLine(void);
};
