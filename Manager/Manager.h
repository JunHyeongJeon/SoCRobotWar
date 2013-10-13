#pragma once
#include "../library/CommProc.h"
#include <queue>
//#include <list>

#define LEN_SEND_CAM 3
#define LEN_SEND_ROBOT 3
#define LEN_RECV_CAM 4
#define LEN_RECV_ROBOT 3
class CManager
{
public:
	CManager();
	~CManager();

	void Process();

private:
	CCommProc *m_pCommProc;

	R_STATE m_cCamTransRobotMotion;
	MV_STEP m_cCamTransRobotMotionCnt;
	int m_cCamTransStepCmd;

	char m_cSendMsg[10];
	char m_cRecvMsg[10];

	bool m_bLineCheck;
	unsigned int m_uiCurrentCnt;
	unsigned int m_uiLineCurrentCnt;

	M_STATE m_eCurrentStep;

	std::queue<M_STATE> m_priOrderList;

	void SetPriOrderList();
	void ReceiveMsg(COMM_TYPE commType);
	void SendMsgCam(M_STATE step, char cnt);
	void SendMsgRobot(R_STATE motion, MV_STEP motion_cnt);
	void DeicideCamState();
	void DeicideCamStateLine();
};
