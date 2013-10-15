#include "Line.h"
#include <vector>
#include <math.h>
#include <list>
#include <algorithm>

CLine::CLine()
{

}

CLine::~CLine()
{

}

bool CLine::MissionTest(_us (*img)[256])
{
	return false;
}

TSEND CLine::Missioning(_us (*img)[256], int now){
	switch(now){
	case 0:
		m_send = Step1(img);
		break;
	}

	return m_send;
}

TSEND CLine::Step1(_us (*img)[256])
{
	TSEND tsend;

	//각도 검사
	if(Gradient(img,tsend) == false) {
		//각도가 안정권에 들어오지 않으면 각도 검사만 하고 종료
		return tsend;
	}

	//각도가 안정권에 있을경우 거리 검사
	Distance(img,tsend);
	return tsend;
}

bool CLine::Gradient(_us (*img)[256],TSEND &tsend)
{
	// 기준 값 ------------------------------------------------------------------
	const float fCriterion = 0; // 기준이 되는 기울기 값
	const float fFlat = 0.07; // 허용되는 기준과의 오차 범위
	// step 설정 ----------------------------------------------------------------
	float fResult = GetGradient(img);
	float fValue = fabs(fResult - fCriterion);
	tsend.step = GetGradientStep(fValue);
	tsend.size = 4;

	printf("[SOC CAM] [ GRADIENT ] [RESULT = [%f]] \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//위에서 구한 거리 값이 기준 보다 작을경우
		tsend.state = R_TURNLEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//위에서 구한 거리 값이 기준 보다 클경우
		tsend.state = R_TURNRIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//안정권안으로 들어옴
		tsend.state = R_WAIT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_WAIT] [SUCCESS] \n");
		return true;
	}
}

bool CLine::Distance(_us (*img)[256],TSEND &tsend)
{
	// 기준 값 ------------------------------------------------------------------
	const float fCriterion = 90;
	const float fFlat = 11.25; // 허용되는 기준과의 오차 범위
	// step 설정 ----------------------------------------------------------------
	float fResult = GetDistance(img);
	float fValue = fabs(fResult - fCriterion);
	tsend.step = GetDistanceStep(fValue);
	tsend.size = 4;
	printf("[SOC CAM] [ DISTANCE ] [RESULT = [%f]] \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//위에서 구한 거리 값이 기준 보다 작을경우
		tsend.state = R_RIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//위에서 구한 거리 값이 기준 보다 클경우
		tsend.state = R_LEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//안정권안으로 들어옴
		tsend.state = R_WAIT;
		tsend.now = (MI_COMP)MI_LINE_END;
		printf("[SOC CAM] ---------- [ DISTANCE ] [R_WAIT] [ SUCCESS ] ---------- \n");
		return true;
	}
}

MV_STEP CLine::GetDistanceStep(float fValue)
{
	if(fValue > 11.25 && fValue <= 16.5)
	{
		return MV_0;
	}
	else if(fValue > 16.5 && fValue <= 22.75)
	{
		return MV_1;
	}
	else if(fValue > 22.75 && fValue <= 28.75)
	{
		return MV_2;
	}
	else if(fValue > 28.75 && fValue <= 34.75)
	{
		return MV_3;
	}
	else if(fValue > 34.75 && fValue <= 40.75)
	{
		return MV_4;
	}
	else if(fValue > 40.75 && fValue <= 45)
	{
		return MV_5;
	}
}

MV_STEP CLine::GetGradientStep(float fValue)
{
	/*
	if(fValue > 0.07 && fValue <= 0.1665)
	{
		return MV_0;
	}
	else if(fValue > 0.1665 && fValue <= 0.333)
	{
		return MV_1;
	}
	else if(fValue > 0.333 && fValue <= 0.4995)
	{
		return MV_2;
	}
	else if(fValue > 0.4995 && fValue <= 0.666)
	{
		return MV_3;
	}
	*/

	return (MV_STEP) ((int)(fabs(fValue / 0.1f) + 0.1f) - 1);
}

float CLine::GetDistance(_us (*img)[256])
{
	std::vector<point> pointList;

	//가운데 10줄을 검사한다.
	for(int x=50;x<130;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				//printf("[DISTANCE] [POINT = [%d] [%d]] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	while(pointList.size() > 2)
	{
		float fSum = 0.f;

		//가로 길이를 모두 구한다.
		for(unsigned int i=0;i<pointList.size();++i)
		{
			fSum += pointList[i].y;
		}

		float fMax = 0;
		int nMaxIndex = 0;
		for(unsigned int i=0;i<pointList.size();++i)
		{
			if(pointList[i].y > fMax)
			{
				fMax = pointList[i].y;
				nMaxIndex = i;
			}
		}

		pointList.erase(pointList.begin() + nMaxIndex);
	}

	if(pointList.size() == 2)
	{	
		float fLength = (pointList[0].y + pointList[1].y) / 2.f;
		//printf("DISTANCE = [%f] \n",fLength);
		return fLength;
	}

	return 0.f;
}

bool Compare(point p1,point p2)
{
	return p1.y > p2.y;
}

float CLine::GetGradient(_us (*img)[256])
{ 
	std::vector<point> vecPoint;
	std::vector<point> vecResultPoint;
	//모든 검은색 point를 구한다.
	for(int x=5;x<175;x+=1)
	{
		bool bFind = false;
		for(int y=115;y>=5;y--)
		{
			_us color = img[y][x];

			if(bFind) {
				if(color != 0x00) {
					printf("[POINT] [(%d , %d)] \n",x,y);
					vecPoint.push_back(point(x,y));
					break;
				}

			}
			else {
				if(color == 0x00) {
					bFind = true;
				}
			}
		}
	}
	
	//중복 되는 점들 제거 
	int nCurrentY = 0;
	for(unsigned int i = 0;i < vecPoint.size();++i)
	{
		if(nCurrentY != vecPoint[i].y) {
			nCurrentY = vecPoint[i].y;
			vecResultPoint.push_back(point(vecPoint[i].x,vecPoint[i].y));
		}
	}

	printf("PROCESS DUPLICATE POINT\n");
	for(unsigned int i = 0;i < vecResultPoint.size();++i)
	{
		printf("[POINT] [(%d , %d)] \n",vecResultPoint[i].x,vecResultPoint[i].y);
	}

	//검색된 포인터가 없거나 1개만 있을경우 0을 반환
	if(vecResultPoint.size() == 0 || vecResultPoint.size() == 1) {
		return 0.f;
	}

	//포인터가 2개가 될때 까지 기울기 차이가 큰값을 제거한다.
	while(vecResultPoint.size() > 2)
	{
		float fSum = 0.f;
		for(unsigned int i = 0;i<vecResultPoint.size() - 1;++i) {
			//fSum += vecPoint[i].y;
			float fGrad = (float)(vecResultPoint[i + 1].y - vecResultPoint[i].y)/(vecResultPoint[i + 1].x - vecResultPoint[i].x);
			fSum += fGrad;
		}

		//기울기 들의 평균을 구함.
		float fAverage = fSum / (vecResultPoint.size() - 1);
		printf("[GRADIENT] [%f] \n",fAverage);

		//평균과 기울기 차가 크게 나는 한 point를 없앤다.
		float fMax = 0;
		int nMaxIndex = 0;
		for(unsigned int i=0;i<vecResultPoint.size()-1;++i)
		{
			float fGrad = (float)(vecResultPoint[i + 1].y - vecResultPoint[i].y)/(vecResultPoint[i + 1].x - vecResultPoint[i].x);
			float fLength = fabs(fGrad - fAverage);
			if(fLength > fMax)
			{
				fMax = fLength;
				nMaxIndex = i;
			}
		}

		vecResultPoint.erase(vecResultPoint.begin() + nMaxIndex);
	}

	//포인터가 2개만 남으면 2개의 기울기를 구한다.
	if(vecResultPoint.size() == 2) {
		float fGrad = (float)(vecResultPoint[1].y - vecResultPoint[0].y)/(vecResultPoint[1].x - vecResultPoint[0].x);
		return fGrad;
	}

	return 0.f;
}