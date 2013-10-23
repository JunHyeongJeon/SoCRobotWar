#pragma once
#include "mission.h"
class CStair :
	public CMission
{
private:
	int m_preUp;
	int m_ishort;
	int m_iLine;
	TSEND m_temp;
	std::list<int>	m_list;
	std::list<int>	m_Llist;
	std::list<int>	m_Rlist;
	int m_stWalk;
	int iMid;
	//////올라가고나서
private:
	int	m_iUp;
	TSEND m_Utemp;
	int m_iGo;
	bool m_bGoing;
public:
	int PreUpMid(_us (*img)[256]);
	bool ChkMid(_us (*img)[256]);
public:
	virtual bool MissionTest(_us (*img)[256]);
	virtual TSEND Missioning(_us (*img)[256], int now);
public:
	TSEND Step1(_us (*img)[256]);
	TSEND Step2(_us (*img)[256]);
	TSEND Step3(_us (*img)[256]);
	TSEND Step4(_us (*img)[256]);
	TSEND Step5(_us (*img)[256]);
public:
	TSEND Up1(_us (*img)[256]);
	TSEND Up2(_us (*img)[256]);
	TSEND Up3(_us (*img)[256]);
	TSEND Up4(_us (*img)[256]);
	bool LineChk2(_us (*img)[256]);

public:
	CStair(void);
	~CStair(void);
};
