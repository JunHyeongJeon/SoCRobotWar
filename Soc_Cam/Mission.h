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
	void SetState(M_STATE state){m_state = state;}
	M_STATE GetState(){return m_state;}
public:
	CMission(void);
	virtual ~CMission(void);
};
