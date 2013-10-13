#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "../library/uart_api.h"

#include "CStateManager.h"
#include "CTimeManager.h"
#include "CRobotControlManager.h"
#include "CMessageCommManager.h"

class CRobotBodyManager
{
public:
	CRobotBodyManager();
	~CRobotBodyManager();
	void Process();
	int Init();
	void init_console();
	void Destroy();
	void ReceiveProcess();

private:

	CStateManager *m_pStateManager;
	CTimeManager *m_pTimeManager;
	struct termios inittio, newtio;
};
