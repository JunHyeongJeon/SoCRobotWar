#pragma once

#include "../library/CommProc.h"
#include <stdio.h>

class CStateManager
{
public:
	CStateManager();
	~CStateManager();

	void Init();
	void Destroy();
	bool SetState(R_STATE nState,MV_STEP nStep);
	R_STATE GetState() {return m_nState;}
	bool Process();

private:

	R_STATE m_nState;
	MV_STEP m_nStep;
	int m_nMotion;
};
