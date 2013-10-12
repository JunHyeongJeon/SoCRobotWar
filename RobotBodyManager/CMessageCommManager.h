#pragma once
#include "../library/CSingleton.h"
#include "../library/CommProc.h"

class CMessageCommManager : public CSingleton<CMessageCommManager>
{
public:
	CMessageCommManager();
	~CMessageCommManager();

	void Connect(COMM_TYPE commType);
	int Send(PIPE_PROTOCOL protocol, char* data,int nSize);
	int Receive(COMM_TYPE commType ,PIPE_PROTOCOL &protocol, char* msg,int nSize);
	CCommProc* GetCommProc();

private:

	CCommProc *m_pCommProc;
};
