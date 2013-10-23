#pragma once
#include "MyHeader.h"
#include "../library/CommProc.h"
class CMission
{
protected:	
	static M_STATE m_state;
	static TSEND	m_send;	
public:		//virtualÇÔ¼öµé
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:

	float GetDegree(_us (*img)[256], _us uTemp);
	MV_STEP GetDegreeStep(float fValue);
	
	void SetState(M_STATE state){m_state = state;}
	M_STATE GetState(){return m_state;}

	float GetDistance(_us (*img)[256],bool bFullLine);
	float GetGradient(_us (*img)[256],bool bFullLine);
	MV_STEP GetDistanceStep(float fValue);
	MV_STEP GetGradientStep(float fValue);
	bool Gradient(_us (*img)[256],TSEND &tsend);
	bool Distance(_us (*img)[256],TSEND &tsend);
	std::vector<LINE> GetPoint(_us (*img)[256],bool bFullLine);
	void doStandardiZation(std::vector<LINE>& vecLine,int nPointCount);
	void printPoint(std::vector<LINE> vecPoint);

public:
	CMission(void);
	virtual ~CMission(void);
};
