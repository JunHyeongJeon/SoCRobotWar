#include <stdlib.h> 
#include <stdio.h> 
#include "CStateManager.h"
#include "CMessageCommManager.h"
#include "CRobotControlManager.h"

CStateManager::CStateManager()
{
	m_nState = R_WAIT;
}

CStateManager::~CStateManager()
{

}

void CStateManager::Destroy()
{

}

void CStateManager::Init()
{
}

bool CStateManager::SetState(R_STATE nState,MV_STEP nStep)
{

	//TKPP("SET STATE = [%s] COUNT = [%d] \n",CMessageCommManager::GetSingleton()->GetCommProc()->GetRStateString(nState),nStep);
	m_nState = nState;
	m_nStep = nStep;

	if(nState == R_WAIT)
		return false;

	switch(nState)
	{
	case R_WAIT:
		m_nMotion = MOTION_NONE;
		break;
	case R_GO:

		//걸어 가는 상태는 Step에 따라서 다른 모션으로 예외 처리
		if(nStep == MV_0) m_nMotion = MOTION_WALK_ONE;
		else if(nStep == MV_1) m_nMotion = MOTION_WALK_ONE;
		else if(nStep == MV_2) m_nMotion = MOTION_WALK_ONE;
		//Step은 0으로 설정
		m_nStep = MV_0;

		break;
	case R_GO_BARI:
		m_nMotion = MOTION_WALK_BARI;
		break;
	case R_LEFT:
		m_nMotion = MOTION_LEFT;
		break;
	case R_RIGHT:
		m_nMotion = MOTION_RIGHT;
		break;
	case R_TURNLEFT:
		m_nMotion = MOTION_TURN_LEFT;
		break;
	case R_TURNRIGHT:
		m_nMotion = MOTION_TURN_RIGHT;
		break;
	case R_TURNLEFT90:
		m_nMotion = MOTION_TURN_LEFT_90;
		break;
	case R_TURNRIGHT90:
		m_nMotion = MOTION_TURN_RIGHT_90;
		break;
	case R_SITDOWN:
		m_nMotion = MOTION_SIT_DOWN;
		break;
	case R_HEAD_LEFT:
		m_nMotion = MOTION_HEAD_LEFT;
		break;
	case R_HEAD_RIGHT:
		m_nMotion = MOTION_HEAD_RIGHT;
		break;
	case R_HEAD_CENTER:
		m_nMotion = MOTION_HEAD_CENTER;
		break;
	case R_LINE:
		m_nMotion = MOTION_LINE;
		break;
	case R_LINE_END:
		m_nMotion = MOTION_LINE_END;
		break;
	case R_CREEP:
		m_nMotion = MOTION_CREEP;
		break;
	
	default:
		m_nMotion = MOTION_NONE;
		break;
	}

	for(int i=0;i<(m_nStep + 1);++i) 
	{
		if(m_nMotion != MOTION_NONE) {
			CRobotControlManager::GetSingleton()->Motion(m_nMotion);
			if(m_nMotion == MOTION_WALK_THREE || m_nMotion == MOTION_SIT_DOWN)
				usleep((int)(c_fStateTime[m_nMotion] * 1000000));
			else
				usleep((int)(c_fStateTime[m_nMotion] * 1000000) + (int)(0.3 * 1000000));
		}	
	}

	return true;
}

bool CStateManager::Process()
{
	return true;
}
