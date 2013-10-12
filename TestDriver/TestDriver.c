#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "uart_api.h"
#include "robot_protocol.h"
#include <list>

void Logo(void)
{
  	printf("\n\n");
  	printf("            *****************************************\n");
  	printf("              Welcome to Eagle Robot Platform Board \n");
  	printf("            *****************************************\n"); 
}

void PrintBannerRobot(void)
{
	printf("\n");
	printf("================================================================\n");
	printf("              <<<< Robot Body TEST Selection >>>>               \n");
	printf("----------------------------------------------------------------\n");
	printf("   1. Robot init           -[a]   |  3. Turn Left	   -[d] \n");
	printf("   2. F_walk		   -[b]   |  4. Turn Right         -[e] \n");
	printf("   3. B_walk		   -[c]   |  5. box		   -[f] \n"); 
	printf("   6. TEST	           -[g]   ");
	printf("----------------------------------------------------------------\n");
	printf("   19. Quit            -[z]         \n");
	printf("================================================================\n");
	
}


int TestItemSelectRobot(void)
{
   char Item;
 	
	while(1) {
 		PrintBannerRobot();
		printf("\nSelected Test Item :  ");
 		Item = getchar();
		putchar(Item);

		switch(Item) {
			case 'a' : case 'A' : init_robot();		break;
			case 'b' : case 'B' : F_walk();			break;
			case 'c' : case 'C' : B_walk();			break;
			case 'd' : case 'D' : Turn_left();		break;
			case 'e' : case 'E' : Turn_right();		break;
			case 'f' : case 'F' : box();			break;
			case 'g' : case 'G' : Test();			break;
			case 'z' : case 'Z' : return 0;
			default : printf("\nNo Test Item Selected");	break;
		}
	}
	return 0;
}

#include <termios.h>
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

int main(void)
{
	std::list<int> testList;

	testList.push_back(1);
	testList.push_back(2);
	testList.push_back(3);
	testList.push_back(4);
	testList.push_back(5);
	testList.push_back(6);
	testList.push_back(7);

	std::list<int>::iterator it = testList.begin();

	
	for(; it != testList.end() ;++it)
	{
		int num = (*it);
		printf(" %d ");
	}
	

	int ret;

	init_console();

	ret = uart_open();
	if (ret < 0) return EXIT_FAILURE;

	uart_config(UART1, 115200, 8, UART_PARNONE, 1);

	Logo();
	TestItemSelectRobot();

	uart_close();

	return 0;
}


