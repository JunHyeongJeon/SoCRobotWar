#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "../library/uart_api.h"
#include "../library/cam_disp.h"
#include "CRobotBodyManager.h"

int main(int argc, char **argv)
{ 	
	CRobotBodyManager* pRobotBodyManager = new CRobotBodyManager;
 	pRobotBodyManager->Init();

 	while(1) {
 		pRobotBodyManager->Process();
 	}

	pRobotBodyManager->Destroy();
 	delete pRobotBodyManager;
	return(0);
}
