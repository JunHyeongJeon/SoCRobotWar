#include "Line.h"
#include <vector>
#include <math.h>
#include <list>

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
	
	//case 1:
	//	m_send = Step2(img);
	//	break;

	}
	return m_send;
}

TSEND CLine::Step1(_us (*img)[256])
{
	TSEND tsend;

	//���� �˻�
	if(Gradient(img,tsend) == false) {
		//������ �����ǿ� ������ ������ ���� �˻縸 �ϰ� ����
		return tsend;
	}

	//������ �����ǿ� ������� �Ÿ� �˻�
	Distance(img,tsend);
	return tsend;
}

bool CLine::Gradient(_us (*img)[256],TSEND &tsend)
{
	// ���� �� ------------------------------------------------------------------
	const float fCriterion = 0; // ������ �Ǵ� ���� ��
	const float fFlat = 0.07; // ���Ǵ� ���ذ��� ���� ����
	// step ���� ----------------------------------------------------------------
	float fResult = GetGradient(img);
	float fValue = fabs(fResult - fCriterion);
	tsend.step = GetGradientStep(fValue);
	tsend.size = 4;
	printf("[SOC CAM] ---------- [ GRADIENT ] [RESULT = [%f]] ---------- \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_TURNLEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] ---------- [ GRADIENT ] [R_LEFT] [STEP = [%d]] ---------- \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_TURNRIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] ---------- [ GRADIENT ] [R_RIGHT] [STEP = [%d]] ---------- \n",tsend.step);
		return false;
	}
	else {
		//�����Ǿ����� ����
		tsend.state = R_WAIT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] ---------- [ GRADIENT ] [R_WAIT] [SUCCESS] ---------- \n");
		return true;
	}
}

bool CLine::Distance(_us (*img)[256],TSEND &tsend)
{
	// ���� �� ------------------------------------------------------------------
	const float fCriterion = 90;
	const float fFlat = 11.25; // ���Ǵ� ���ذ��� ���� ����
	// step ���� ----------------------------------------------------------------
	float fResult = GetDistance(img);
	float fValue = fabs(fResult - fCriterion);
	tsend.step = GetDistanceStep(fValue);
	tsend.size = 4;
	printf("[SOC CAM] ---------- [ DISTANCE ] [RESULT = [%f]] ---------- \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_RIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] ---------- [ DISTANCE ] [R_RIGHT] [STEP = [%d]] ---------- \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_LEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] ---------- [ DISTANCE ] [R_LEFT] [STEP = [%d]] ---------- \n",tsend.step);
		return false;
	}
	else {
		//�����Ǿ����� ����
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

	return (MV_STEP) ((int)(fabs(fValue / 0.1f) + 0.1f));
}

float CLine::GetDistance(_us (*img)[256])
{
	std::vector<point> pointList;

	//��� 10���� �˻��Ѵ�.
	for(int x=50;x<130;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				printf("[DISTANCE] [POINT = [%d] [%d]] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	while(pointList.size() > 2)
	{
		float fSum = 0.f;

		//���� ���̸� ��� ���Ѵ�.
		for(unsigned int i=0;i<pointList.size();++i)
		{
			fSum += pointList[i].y;
		}

		//���� ���̵��� ����� ���Ѵ�.
		//float fAverage = fSum / pointList.size();

		//��հ� ũ�� ���̳��� ������ ã�Ƽ� PID��� �Ѵ�.
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
		printf("DISTANCE = [%f] \n",fLength);
		return fLength;
	}

	return 0.f;
}

float CLine::GetGradient(_us (*img)[256])
{ 
	std::vector<LINE> vecLine;

	//��� ������ point�� ���Ѵ�.
	for(int x=10;x<170;x+=5)
	{
		bool bFind = false;
		point p1;
		point p2;

		for(int y=110;y>10;y--)
		{
			_us color = img[y][x];			
			if(bFind == false) {
				if(color == 0x00) {
					p1.x = x;
					p1.y = y;
					bFind = true;
				}
			}
			else {
				if(color != 0x00) {
					p2.x = x;
					p2.y = y;
					vecLine.push_back(LINE(p1,p2));
					printf("[GRADIENT] [p1(%d,%d)] [p2(%d,%d)] [FL(%d)] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
					break;
				}
			}
		}
	}

	for(unsigned int i = 0; i < vecLine.size();i++)
	{
		//�β��� ��� �β����� ���� �� ���� ũ�ų� ������� ����
		if((vecLine[i].p1.y - vecLine[i].p2.y) < 5 || (vecLine[i].p1.y - vecLine[i].p2.y) > 15)
		{
			vecLine.erase(vecLine.begin() + i);
			i--;
		}
	}

	printf("[ RESULT ] \n");
	for(unsigned int i = 0;i< vecLine.size();++i)
	{
		printf("[GRADIENT] [p1(%d,%d)] [p2(%d,%d)] [FL(%d)] \n",vecLine[i].p1.x,vecLine[i].p1.y,vecLine[i].p2.x,vecLine[i].p2.y,vecLine[i].p1.y - vecLine[i].p2.y);
	}

	//p2�� �������� �������̰� ���̳��� ���� ���� �Ͽ� 2���� �����͸� �����.
	while(vecLine.size() > 2)
	{
		std::vector<float> gradList;
		float fSum = 0;

		//��� ���⸦ ����
		for(unsigned int i=0;i<vecLine.size() - 1;++i)
		{
			int nDx = vecLine[i + 1].p2.x - vecLine[i].p2.x;
			int nDy = vecLine[i + 1].p2.y - vecLine[i].p2.y;
			nDx == 0 ? nDx = 1 : nDx;

			float fGrad = (float)nDy / nDx;
			gradList.push_back(fGrad);
			fSum += fGrad;
		}

		//���� ���� ����� ����
		float fAverage = fSum / (vecLine.size() - 1);

		//��հ� ���� ���� ũ�� ���� �� point�� ���ش�.
		float fMax = 0;
		int nMaxIndex = 0;
		for(unsigned int i=0;i<gradList.size();++i)
		{
			float fGrad = gradList[i];
			float fLength = fabs(fGrad - fAverage);

			if(fLength > fMax)
			{
				fMax = fLength;
				nMaxIndex = i;
			}
		}

		vecLine.erase(vecLine.begin() + nMaxIndex);
	}

	//���� ��հ� �ٻ��� 2���� �����͸� �̿��� ���⸦ ���Ѵ�.
	if(vecLine.size() == 2)
	{
		int nDx = vecLine[1].p2.x - vecLine[0].p2.x;
		int nDy = vecLine[1].p2.y - vecLine[0].p2.y;

		nDx == 0 ? nDx = 1 : nDx;
		float fGrad = (float)nDy / nDx;
		printf("GRADIENT = [%f] \n",fGrad);
		return fGrad;
	}

	return 0.f;
}
