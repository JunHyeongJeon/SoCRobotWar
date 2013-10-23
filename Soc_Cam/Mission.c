#include "Mission.h"

#define GRADIENT_SAFETY_ZONE 0.09
#define DISTANCE_SAFETY_ZONE 5
#define GRADIENT_SAFETY_CRITERION 0
#define DISTANCE_SAFETY_CRITERION 90

M_STATE CMission::m_state = M_BARI1;
TSEND CMission::m_send;
CMission::CMission(void)
{
}

CMission::~CMission(void)
{
}

bool CMission::MissionTest(_us (*img)[256]){
	return true;
}

TSEND CMission::Missioning(_us (*img)[256], int now){
	return m_send;
}

float CMission::GetDegree(_us (*img)[256], _us uTemp)
{ 
	bool bFullLine = true;
	return GetGradient(img,bFullLine);
}

MV_STEP CMission::GetDegreeStep(float fValue)
{
	GetGradientStep(fValue);
}

bool CMission::Gradient(_us (*img)[256],TSEND &tsend)
{
	bool bFullLine = false;
	float fResult = GetGradient(img,bFullLine);
	float fValue = fabs(fResult - GRADIENT_SAFETY_CRITERION);
	int iGMax = 0;
	iGMax = (int)GetGradientStep(fValue);
	if(iGMax > 3)
		iGMax = 3;

	
	tsend.step = (MV_STEP)iGMax; tsend.size = 4;

	printf("[SOC CAM] [ GRADIENT ] [RESULT = [%f]] \n",fResult);
	if(fResult < (GRADIENT_SAFETY_CRITERION - GRADIENT_SAFETY_ZONE))
	{
		//위에서 구한 거리 값이 기준 보다 작을경우
		iGMax += 2;
		tsend.step = (MV_STEP)iGMax;
		tsend.state = R_TURNLEFT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (GRADIENT_SAFETY_CRITERION + GRADIENT_SAFETY_ZONE))
	{
		//위에서 구한 거리 값이 기준 보다 클경우
		tsend.state = R_TURNRIGHT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//안정권안으로 들어옴
		tsend.state = R_WAIT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_WAIT] [SUCCESS] \n");
		return true;
	}
}

bool CMission::Distance(_us (*img)[256],TSEND &tsend)
{
	bool bFullLine = false;
	float fResult = GetDistance(img,bFullLine);
	float fValue = fabs(fResult - DISTANCE_SAFETY_CRITERION);
	int iMax = 0;
	iMax = (int)GetDistanceStep(fValue);
	if(iMax > 3)
		iMax = 3;
	tsend.step = (MV_STEP)iMax; tsend.size = 4;
	printf("[SOC CAM] [ DISTANCE ] [RESULT = [%f]] \n",fResult);

	if(fResult < (DISTANCE_SAFETY_CRITERION - DISTANCE_SAFETY_ZONE))
	{
		//위에서 구한 거리 값이 기준 보다 작을경우
		tsend.state = R_RIGHT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (DISTANCE_SAFETY_CRITERION + DISTANCE_SAFETY_ZONE))
	{
		//위에서 구한 거리 값이 기준 보다 클경우
		tsend.state = R_LEFT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//안정권안으로 들어옴
		tsend.state = R_WAIT; tsend.now = (MI_COMP)MI_LINE_END;
		printf("[SOC CAM] ---------- [ DISTANCE ] [R_WAIT] [ SUCCESS ] ---------- \n");
		return true;
	}
}

MV_STEP CMission::GetDistanceStep(float fValue)
{
	int nCount = (int)fabs(fValue / 15);
	if(nCount > 5) {
		return (MV_STEP)5;
	}

	return (MV_STEP)nCount;
}

MV_STEP CMission::GetGradientStep(float fValue)
{
	int nCount = (int)fabs(fValue / 0.1924f);
	if(nCount > 5) {
		return (MV_STEP)5;
	}

	return (MV_STEP)nCount;
}

void CMission::printPoint(std::vector<LINE> vecPoint)
{
	for(unsigned int i = 0;i <vecPoint.size(); ++ i) 
	{
		printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",vecPoint[i].p1.x,vecPoint[i].p1.y,vecPoint[i].p2.x,vecPoint[i].p2.y,vecPoint[i].p1.y - vecPoint[i].p2.y);
	}
}

std::vector<LINE> CMission::GetPoint(_us (*img)[256],bool bFullLine)
{
	std::vector<LINE> vecLine;
	for(int x=20;x<160;x+=1)
	{
		bool bFind = false;
		point p1;
		point p2;
		for(int y=100;y>=20;y--)
		{
			_us color = img[y][x];
			if(bFind) {
				if(color != 0x00) {
					p2 = point(x,y);
					vecLine.push_back(LINE(p1,p2));
					break;
				}
			}
			else {
				if(color == 0x00) {
					bFind = true;
					p1 = point(x,y);
				}
			}
		}
	}

	std::vector<LINE> vecResultLine;
	for(unsigned int i = 0;i < vecLine.size();++i)
	{
		point p1 = vecLine[i].p1;
		point p2 = vecLine[i].p2;
		float fLength = p1.y - p2.y;

		if(bFullLine)
		{
			if(fLength >=5 && fLength <= 30) {
				if(p1.y >= 90 ) {
					if(p1.x >= 90) {
						vecResultLine.push_back(vecLine[i]);
					}
				}
				else {
					vecResultLine.push_back(vecLine[i]);
				}
			}
		}
		else {
			vecResultLine.push_back(vecLine[i]);
		}
	}

	return vecResultLine;
}

void CMission::doStandardiZation(std::vector<LINE>& vecLine,int nPointCount)
{
	//포인터가 nPointCount만큼 남을때 까지 기울기 차이가 큰값을 제거한다.
	while(vecLine.size() > nPointCount)
	{
		float fSum = 0.f;
		for(unsigned int i = 0;i<vecLine.size() - 1;++i) {
			float fGrad = (float)(vecLine[i + 1].p1.y - vecLine[i].p1.y)/(vecLine[i + 1].p1.x - vecLine[i].p1.x);
			fSum += fGrad;
		}

		//기울기 들의 평균을 구함.
		float fAverage = fSum / (vecLine.size() - 1);
		//printf("[GRADIENT] [%f] \n",fAverage);

		//평균과 기울기 차가 크게 나는 한 point를 없앤다.
		float fMax = 0;
		int nMaxIndex = 0;
		for(unsigned int i=0;i<vecLine.size()-1;++i)
		{
			float fGrad = (float)(vecLine[i + 1].p1.y - vecLine[i].p1.y)/(vecLine[i + 1].p1.x - vecLine[i].p1.x);
			float fLength = fabs(fGrad - fAverage);
			if(fLength > fMax)
			{
				fMax = fLength;
				nMaxIndex = i;
			}
		}
		vecLine.erase(vecLine.begin() + nMaxIndex);
	}
}


float CMission::GetDistance(_us (*img)[256],bool bFullLine)
{
	std::vector<LINE> vecLine = GetPoint(img,bFullLine);

	//검색된 포인터가 없거나 1개만 있을경우 0을 반환
	if(vecLine.size() == 0) {
		return 0.f;
	}

	if(vecLine.size() == 1) {
		return vecLine[0].p1.y;
	}

	doStandardiZation(vecLine,2);

	//포인터가 2개만 남으면 2개의 평균 높이를 구한다.
	if(vecLine.size() == 2) {
		float fLength = (vecLine[0].p1.y + vecLine[1].p1.y) / 2.f;
		return fLength;
	}

	printf(" DISTANCE RETURN 0 \n");
	return 0.f;
}

float CMission::GetGradient(_us (*img)[256],bool bFullLine)
{ 
	std::vector<LINE> vecLine = GetPoint(img,bFullLine);

	//검색된 포인터가 없거나 1개만 있을경우 0을 반환
	if(vecLine.size() == 0 || vecLine.size() == 1) {
		return 0.f;
	}

	printPoint(vecLine);
	doStandardiZation(vecLine,2);

	//포인터가 2개만 남으면 2개의 기울기를 구한다.
	if(vecLine.size() == 2) {
		float fGrad = (float)(vecLine[1].p1.y - vecLine[0].p1.y)/(vecLine[1].p1.x - vecLine[0].p1.x);
		return fGrad;
	}

	return 0.f;
}

/*
	std::vector<LINE> vecLine;
	for(int x=5;x<175;x+=1)
	{
		bool bFind = false;
		point p1;
		point p2;
		for(int y=115;y>=5;y--)
		{
			_us color = img[y][x];
			if(bFind) {
				if(color != uTemp) {
					p2 = point(x,y);
					printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
					vecLine.push_back(LINE(p1,p2));
					break;
				}
			}
			else {
				if(color == uTemp) {
					bFind = true;
					p1 = point(x,y);
				}
			}
		}
	}

	std::vector<LINE> vecResultLine;
	int nCurrentY = 0;
	for(unsigned int i = 0;i < vecLine.size();++i)
	{
		if(nCurrentY != vecLine[i].p1.y ) {
			nCurrentY = vecLine[i].p1.y;
			vecResultLine.push_back(vecLine[i]);
		}
	}

	printf("PROCESS DUPLICATE POINT\n");
	for(unsigned int i = 0;i < vecResultLine.size();++i)
	{
		point p1 = vecResultLine[i].p1;
		point p2 = vecResultLine[i].p2;
		printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
	}

	//검색된 포인터가 없거나 1개만 있을경우 0을 반환
	if(vecResultLine.size() == 0 || vecResultLine.size() == 1) {
		return 0.f;
	}

	//포인터가 2개가 될때 까지 기울기 차이가 큰값을 제거한다.

	while(vecResultLine.size() > 2)
	{
		float fSum = 0.f;
		for(unsigned int i = 0;i<vecResultLine.size() - 1;++i) {
			float fGrad = (float)(vecResultLine[i + 1].p1.y - vecResultLine[i].p1.y)/(vecResultLine[i + 1].p1.x - vecResultLine[i].p1.x);
			fSum += fGrad;
		}

		//기울기 들의 평균을 구함.
		float fAverage = fSum / (vecResultLine.size() - 1);
		printf("[GRADIENT] [%f] \n",fAverage);

		//평균과 기울기 차가 크게 나는 한 point를 없앤다.
		float fMax = 0;
		int nMaxIndex = 0;
		for(unsigned int i=0;i<vecResultLine.size()-1;++i)
		{
			float fGrad = (float)(vecResultLine[i + 1].p1.y - vecResultLine[i].p1.y)/(vecResultLine[i + 1].p1.x - vecResultLine[i].p1.x);
			float fLength = fabs(fGrad - fAverage);
			if(fLength > fMax)
			{
				fMax = fLength;
				nMaxIndex = i;
			}
		}
		vecResultLine.erase(vecResultLine.begin() + nMaxIndex);
	}

	//포인터가 2개만 남으면 2개의 기울기를 구한다.
	if(vecResultLine.size() == 2) {
		float fGrad = (float)(vecResultLine[1].p1.y - vecResultLine[0].p1.y)/(vecResultLine[1].p1.x - vecResultLine[0].p1.x);
		return fGrad;
	}
	*/


//return (MV_STEP)((int)fabs(fValue / 0.1924f));