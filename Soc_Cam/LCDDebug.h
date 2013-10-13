#pragma once
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <vector>
#include "MyHeader.h"
#include "../library/CommProc.h"

struct point
{
	int x;
	int y;

	point(int _x,int _y) 
	{
		x = _x;
		y = _y;
	}
};

void LCDDebug(_us (*img)[256],M_STATE state);
float GetGradient(_us (*img)[256]);
float GetDistance(_us (*img)[256]);
