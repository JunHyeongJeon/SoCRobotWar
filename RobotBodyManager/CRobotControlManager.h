#pragma once

#include "../library/CSingleton.h"

#define Header0	0xff
#define Header1 0xff
#define Packet	0x09
#define pID	0xfd
#define CMD	0x16
#define DATA1	0x00

class CRobotControlManager :public CSingleton< CRobotControlManager >
{
public:
	CRobotControlManager();
	~CRobotControlManager();

	void Init();
	void DelayLoop(int delay_time);
	void init_robot();
	void Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0);
	void Motion(unsigned char DATA0);

private:

};
