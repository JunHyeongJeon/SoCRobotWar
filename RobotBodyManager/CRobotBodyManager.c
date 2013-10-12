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
	//B115200  초당 115200 비트 의 속도로 통신을 함
	cfsetispeed(&newtio, B115200);
	tcsetattr(0, TCSANOW, &newtio);
}

int CRobotBodyManager::Init()
{

	int ret;
	init_console();
	ret = uart_open();
	if (ret < 0) {
		//uart 연결 실패시
		printf("FAIL : UART \n");
	}

	uart_config(UART1, 115200, 8, UART_PARNONE, 1); //uart 채널 설정
	m_pStateManager->Init(); //state manager 초기화
	CMessageCommManager::GetSingleton()->Connect(COMM_TYPE_ROT_TO_MGR); // rot -> mgr 파이프 연결
	CMessageCommManager::GetSingleton()->Connect(COMM_TYPE_MGR_TO_ROT); // mgr -> rot 파이프 연결
	CRobotControlManager::GetSingleton()->Init(); //robot control manager 초기화
	m_pTimeManager->Init(); //timer manager 초기화

	return 0;
}

void CRobotBodyManager::Process()
{
	//프레임마다 경과된 시간을 검사한다.
	double dTime = 0;
	dTime = m_pTimeManager->Process();

	//STAY 상태 일때 다른 메세지를 기다린다.
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
					//안쓰고 있으면 빼도 될거 같음	
					//sendBuff[0] = REQ_STATE_CHANGE_ACK_OK;
					//CMessageCommManager::GetSingleton()->Send(PP_ROBOT_STATE_CHANGE_ACK,sendBuff);

					//받은 State 상태 변경
					R_STATE nState = (R_STATE)receiveBuff[1];
					MV_STEP nStep = (MV_STEP)receiveBuff[2];
					m_pStateManager->SetState(nState,nStep);

					//State 완료 됨을 manager에게 전송
					CMessageCommManager::GetSingleton()->Send(PP_ROBOT_STATE_END,receiveBuff,MESSAGE_SIZE_ROT_TO_MGR);
				
					//상태를 NONE 상태로 변경
					m_pStateManager->SetState(R_WAIT,MV_0);
				}
				break;
			default:
				break;
		}
	}
}
