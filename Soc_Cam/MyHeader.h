#pragma once

#include <vector>

#include <math.h>
#include <list>
#include <algorithm>
enum BARIGATE {BI_GARO, BI_SERO};
enum MT_STATE {MT_FAIL, MT_ING, MT_SUCCESS};
typedef unsigned short _us;
typedef unsigned short* _pus;
static _us		_Red	= 0xF800;
static _us		_Green	= 0x07E0;
static _us		_Blue	= 0x001F;
static _us      _Yellow = _Red | _Green;
static _us      _Black = 0x0000;
static _us      _White = 0xFFFF;

struct point
{
	int x;
	int y;

	point(int _x,int _y) 
	{
		x = _x;
		y = _y;
	}

	point()
	{

	}
};

struct LINE
{
	point p1;
	point p2;

	LINE(point _p1,point _p2)
	{
		p1 = _p1;
		p2 = _p2;
	};
};