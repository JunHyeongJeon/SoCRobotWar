#include "CMessageCommManager.h"

CMessageCommManager::CMessageCommManager()
{
	m_pCommProc = new CCommProc;
}

CMessageCommManager::~CMessageCommManager()
{
	delete m_pCommProc;
}

void CMessageCommManager::Connect(COMM_TYPE commType)
{
	m_pCommProc->Connect(commType);
}

CCommProc* CMessageCommManager::GetCommProc()
{
	return m_pCommProc;
}

int CMessageCommManager::Send(PIPE_PROTOCOL protocol, char *data,int nSize)
{
	//TKPP("[ROBOT BODY] [SEND] [PROTOCOL = [%s]] \n",m_pCommProc->GetProtocolString(protocol));
	data[0] = protocol;
	return m_pCommProc->SendMessage(data,nSize);
}

int CMessageCommManager::Receive(COMM_TYPE commType ,PIPE_PROTOCOL &protocol, char *msg,int nSize)
{	
	int result = m_pCommProc->ReceiveMessage(commType,msg,nSize);
	protocol = (PIPE_PROTOCOL)msg[0];

	//TKPP("[ROBOT BODY] [RECEIVE] [PROTOCOL = [%s]] \n",m_pCommProc->GetProtocolString(protocol));
	for(int i=0;i<nSize;++i)
	{
		//TKPP("MSG[%d] = [%d]\n",i,(int)msg[i]);
	}
	return result;
}
