#include "CRobotControlManager.h"
#include <stdio.h>
#include <string.h>
#include "../library/uart_api.h"
#include "../library/pipeline_api.h"

CRobotControlManager::CRobotControlManager()
{

}

CRobotControlManager::~CRobotControlManager()
{

}

void CRobotControlManager::Init()
{
	init_robot();
}

void CRobotControlManager::DelayLoop(int delay_time)
{
	while(delay_time) 
		delay_time--;
}

void CRobotControlManager::init_robot()
{
	unsigned char Init_Buffer[10] = {0,};
	Init_Buffer[0] = 0xff;
	Init_Buffer[1] = 0xff;
	Init_Buffer[2] = 0x0a;
	Init_Buffer[3] = 0xfe;
	Init_Buffer[4] = 0x03;
	Init_Buffer[5] = 0xa2;
	Init_Buffer[6] = 0x5c;
	Init_Buffer[7] = 0x34;
	Init_Buffer[8] = 0x01;
	Init_Buffer[9] = 0x60;

	uart1_buffer_write(Init_Buffer, 10);
}

void CRobotControlManager::Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0)
{
	unsigned char Command_Buffer[9] = {0,};

	Command_Buffer[0] = 0xff;	
	Command_Buffer[1] = 0xff;
	Command_Buffer[2] = 0x09;
	Command_Buffer[3] = 0xfd;
	Command_Buffer[4] = 0x16;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;
	Command_Buffer[7] = DATA0;
	Command_Buffer[8] = 0x00;

	uart1_buffer_write(Command_Buffer, 9);
}

void CRobotControlManager::Motion(unsigned char DATA0)
{
	unsigned char CS1;
	unsigned char CS2;

	CS1 = (Packet^pID^CMD^DATA0^DATA1) & 0xfe;
	CS2 = (~(Packet^pID^CMD^DATA0^DATA1)) & 0xfe;
	Send_Command(CS1, CS2, DATA0);
	
	printf("DATA0=%x\n", DATA0);
	printf("CS1=%x\n", CS1);
	printf("CS2=%x\n", CS2);
	//DelayLoop(15000000);		// 3second delay
}
