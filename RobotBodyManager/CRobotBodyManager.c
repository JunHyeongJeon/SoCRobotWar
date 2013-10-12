#include "CRobotBodyManager.h"

CRobotBodyManager::CRobotBodyManager()
{
	m_pStateManager = new CStateManager;
	m_pTimeManager = new CTimeManager;

	CRobotControlManager::CreateSingleton();
	CMessageCommManager::CreateSingleton();
}

CRobotBodyManager::~CRobotBodyManager()
{
	delete m_pStateManager;

	delete m_pTimeManager;

	CRobotControlManager::DestroySingleton();
	CMessageCommManager::DestroySingleton();
}

void CRobotBodyManager::Destroy()
{
	uart_close();
}

void CRobotBodyManager::init_console()
{
	tcgetattr(0, &inittio);
	newtio = inittio;
	newtio.c_lflag &= ~ICANON;
	newtio.c_lflag &= ~ECHO;
	newtio.c_lflag &= ~ISIG;
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;
	//B115200  �ʴ� 115200 ��Ʈ �� �ӵ��� ����� ��
	cfsetispeed(&newtio, B115200);
	tcsetattr(0, TCSANOW, &newtio);
}

int CRobotBodyManager::Init()
{

	int ret;
	init_console();
	ret = uart_open();
	if (ret < 0) {
		//uart ���� ���н�
		printf("FAIL : UART \n");
	}

	uart_config(UART1, 115200, 8, UART_PARNONE, 1); //uart ä�� ����
	m_pStateManager->Init(); //state manager �ʱ�ȭ
	CMessageCommManager::GetSingleton()->Connect(COMM_TYPE_ROT_TO_MGR); // rot -> mgr ������ ����
	CMessageCommManager::GetSingleton()->Connect(COMM_TYPE_MGR_TO_ROT); // mgr -> rot ������ ����
	CRobotControlManager::GetSingleton()->Init(); //robot control manager �ʱ�ȭ
	m_pTimeManager->Init(); //timer manager �ʱ�ȭ

	return 0;
}

void CRobotBodyManager::Process()
{
	//�����Ӹ��� ����� �ð��� �˻��Ѵ�.
	double dTime = 0;
	dTime = m_pTimeManager->Process();

	//STAY ���� �϶� �ٸ� �޼����� ��ٸ���.
	if(m_pStateManager->GetState() == R_WAIT) {
		ReceiveProcess();
	} 
}

void CRobotBodyManager::ReceiveProcess()
{
	char receiveBuff[MESSAGE_SIZE_ROT_TO_MGR];
	PIPE_PROTOCOL protocol = PP_NONE;

	if(CMessageCommManager::GetSingleton()->Receive(COMM_TYPE_MGR_TO_ROT,protocol,receiveBuff,MESSAGE_SIZE_ROT_TO_MGR) == 0) {
		switch(protocol)
		{
			case PP_ROBOT_STATE_CHANGE:
				{
					//�Ⱦ��� ������ ���� �ɰ� ����	
					//sendBuff[0] = REQ_STATE_CHANGE_ACK_OK;
					//CMessageCommManager::GetSingleton()->Send(PP_ROBOT_STATE_CHANGE_ACK,sendBuff);

					//���� State ���� ����
					R_STATE nState = (R_STATE)receiveBuff[1];
					MV_STEP nStep = (MV_STEP)receiveBuff[2];
					m_pStateManager->SetState(nState,nStep);

					//State �Ϸ� ���� manager���� ����
					CMessageCommManager::GetSingleton()->Send(PP_ROBOT_STATE_END,receiveBuff,MESSAGE_SIZE_ROT_TO_MGR);
				
					//���¸� NONE ���·� ����
					m_pStateManager->SetState(R_WAIT,MV_0);
				}
				break;
			default:
				break;
		}
	}
}
