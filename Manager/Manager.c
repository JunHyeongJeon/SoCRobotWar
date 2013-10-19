#include "Manager.h"
#include "../library/CommProc.h"

CManager::CManager()
{
	m_pCommProc = new CCommProc();

	m_uiLineCurrentCnt = 0;
	m_uiCurrentCnt = 0;
	m_bLineCheck = false;

	SetPriOrderList();

	m_pCommProc->Connect(COMM_TYPE_CAM_TO_MGR);
	m_pCommProc->Connect(COMM_TYPE_MGR_TO_CAM);
	m_pCommProc->Connect(COMM_TYPE_MGR_TO_ROT);
	m_pCommProc->Connect(COMM_TYPE_ROT_TO_MGR);
}

CManager::~CManager()
{
	delete m_pCommProc;
}

void CManager::SetPriOrderList()
{
	/*
	int inum = 0;
	int iList = 0;
	FILE *fp = fopen("OrderList.txt", "r");
	if( fp == NULL){
		printf("ERROR\n");
	}
	inum = fgetc(fp);
	iList = fgetc(fp);
	for(int i = 0; i < inum; ++i){
		iList = fgetc(fp);
		m_priOrderList.push((M_STATE)iList);
		iList = fgetc(fp);
	}

	fclose(fp);
	*/

// 	m_priOrderList.push(M_BARI1);
// 	m_priOrderList.push(M_HURDLE);

// 	m_priOrderList.push(M_EDGE);
// 	m_priOrderList.push(M_TURNEL1);
// 	m_priOrderList.push(M_STAIR);
// 	m_priOrderList.push(M_TURNEL2);
// 	m_priOrderList.push(M_GOLF);
 	/*m_priOrderList.push(M_RED);*/
// 	m_priOrderList.push(M_BARI2);

	//�ӽ� �׽�Ʈ��
	m_priOrderList.push(M_ALIGN_CENTER);
	//m_priOrderList.push(M_TURNEL1);
// 
// 	m_priOrderList.push(M_RED);
	/*m_priOrderList.push(M_TURNEL1);*/

}

void CManager::Process()
{
	if(m_priOrderList.empty())
		return;

	m_eCurrentStep = m_priOrderList.front(); 	// ����Ǿ�� �� ��ֹ�
	if(m_eCurrentStep == M_ALIGN_CENTER) {
		m_bLineCheck = true;
	}

	//TKPPprintf("\n\n**********     %s     **********\n", 
	//TKPP	m_pCommProc->GetMStateString(m_eCurrentStep));

	if(m_bLineCheck == false)
	{
		// ���� ó�� ��� ��,����
		SendMsgCam(m_eCurrentStep, m_uiCurrentCnt);
		ReceiveMsg(COMM_TYPE_CAM_TO_MGR);

		// ķ ���� ����
		DeicideCamState();

		// �κ� ��� ��� ���� ��� ����
		SendMsgRobot(m_cCamTransRobotMotion, m_cCamTransRobotMotionCnt);
		ReceiveMsg(COMM_TYPE_ROT_TO_MGR);
	}
	else if(m_bLineCheck == true) 
	{
		// �κ� ��� ��� ���� (���� ���� �ൿ)
		SendMsgRobot(R_LINE,MV_0);
		ReceiveMsg(COMM_TYPE_ROT_TO_MGR); // END

		SendMsgCam(M_ALIGN_CENTER, m_uiLineCurrentCnt);
		ReceiveMsg(COMM_TYPE_CAM_TO_MGR);
		
		DeicideCamStateLine();

		//���� ���� ��� ����
		SendMsgRobot(R_LINE_END,MV_0);
		ReceiveMsg(COMM_TYPE_ROT_TO_MGR); // END

		// �κ� ��� ��� ���� (�߾� ���� �ൿ)
		SendMsgRobot(m_cCamTransRobotMotion, m_cCamTransRobotMotionCnt);
		ReceiveMsg(COMM_TYPE_ROT_TO_MGR); // END
	}
}

void CManager::DeicideCamStateLine()
{
	switch((MI_LINE_COMP)m_cCamTransStepCmd)
	{
		// ���� �ܰ� �״�� ��� ����
	case MI_LINE_NOW:
		break;
	case MI_LINE_NEXT:
		m_uiLineCurrentCnt++;
		break;
		// ���� �߾� ���� �ϼ�
	case MI_LINE_END:
		m_bLineCheck = false;
		m_uiLineCurrentCnt = 0;

		if(m_eCurrentStep == M_ALIGN_CENTER)
		{
			m_uiCurrentCnt = 0;
			m_priOrderList.pop();
		}
		break;
	}
}

void CManager::DeicideCamState()
{
	switch((MI_COMP)m_cCamTransStepCmd)
	{
	// ���� �ܰ�� ���ư�
	case MI_PREV:
		if(m_uiCurrentCnt > 0)	
			m_uiCurrentCnt--;
		break;
	// ���� �ܰ� �״�� ��� ����
	case MI_NOW:
		break;
	// ���� �ܰ�� ����
	case MI_NEXT:
		m_uiCurrentCnt++;
		break;
	// ������ ���� �߾� ���� ����
	case MI_LINE:
		m_uiCurrentCnt++;
		m_bLineCheck = true;
		break;
	// ��ֹ� �̼� Ŭ����
	case MI_END:
		m_uiCurrentCnt = 0;
		m_priOrderList.pop();
		break;
	}
}

void CManager::SendMsgCam(M_STATE step, char cnt)
{
	char msg[LEN_SEND_CAM];
	msg[0] = PP_CAM_CAPTURE;
	msg[1] = step;
	msg[2] = cnt;
	m_pCommProc->SendMessage(msg, LEN_SEND_CAM);

	//TKPP("[Manager] SEND : CAM\n");
	//TKPP("[Manager] Mission Num = %s\n", m_pCommProc->GetMStateString(step));
	//TKPP("[Manager] Count = %d\n\n", cnt);
}

void CManager::SendMsgRobot(R_STATE motion, MV_STEP motion_cnt)
{
	char msg[LEN_SEND_ROBOT];
	msg[0] = PP_ROBOT_STATE_CHANGE;
	msg[1] = motion;
	msg[2] = motion_cnt;
	m_pCommProc->SendMessage(msg, LEN_SEND_ROBOT);

	//TKPP("[Manager] SEND : ROBOT\n");
	//TKPP("[Manager] Motion Num = %d\n", msg[1]);
	//TKPP("[Manager] Motion Count = %d\n\n",  msg[2]);
}

void CManager::ReceiveMsg(COMM_TYPE commType)
{
	int size;

	size = (commType == COMM_TYPE_CAM_TO_MGR) ? LEN_RECV_CAM : LEN_RECV_ROBOT;
	m_pCommProc->ReceiveMessage(commType, m_cRecvMsg, size);

	switch((PIPE_PROTOCOL)m_cRecvMsg[0])
	{
	// CAM ��� ����
	case PP_CAM_RESULT:
		//TKPP("[Manager] RECV : CAM_RESULT\n");
		m_cCamTransRobotMotion = (R_STATE)m_cRecvMsg[1];	// �κ��� ������
		m_cCamTransRobotMotionCnt = (MV_STEP)m_cRecvMsg[2];	// �κ��� ������ ī��Ʈ
		m_cCamTransStepCmd = (MI_COMP)m_cRecvMsg[3];		// ���� ����
		//TKPP("[Manager] m_cCamTransRobotMotion = %d\n", m_cRecvMsg[1]);
		//TKPP("[Manager] m_cCamTransRobotMotionCnt = %d\n", m_cRecvMsg[2]);
		//TKPP("[Manager] m_cCamTransStepCmd = %d\n\n", m_cRecvMsg[3]);
		break;
	// ��� ���� Ȯ�� �۾�
	case PP_ROBOT_STATE_CHANGE_ACK:
		//TKPP("[Manager] RECV : ROBOT_STATE_CHANGE_ACK\n\n");
		break;
	// ��� ��
	case PP_ROBOT_STATE_END:
		//TKPP("[Manager] RECV : ROBOT_STATE_END\n\n");
		break;
	default:
		//TKPP("[Manager] RECV : DEFAULT(ERROR)\n\n");
		break;
	}
}
