#include "Line.h"


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

	printf("[SOC CAM] [ GRADIENT ] [RESULT = [%f]] \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_TURNLEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_TURNRIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//�����Ǿ����� ����
		tsend.state = R_WAIT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_WAIT] [SUCCESS] \n");
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
	printf("[SOC CAM] [ DISTANCE ] [RESULT = [%f]] \n",fResult);
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_RIGHT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (fCriterion + fFlat))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_LEFT;
		tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
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

	return (MV_STEP)((int)fabs(fValue / 0.1924f));
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
				//printf("[DISTANCE] [POINT = [%d] [%d]] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	while(pointList.size() > 2)
	{
		float fSum = 0.f;
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
		return fLength;
	}

	return 0.f;
}

float CLine::GetGradient(_us (*img)[256])
{ 
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
				if(color != 0x00) {
					p2 = point(x,y);
					printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
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

	//�˻��� �����Ͱ� ���ų� 1���� ������� 0�� ��ȯ
	if(vecResultLine.size() == 0 || vecResultLine.size() == 1) {
		return 0.f;
	}

	//�����Ͱ� 2���� �ɶ� ���� ���� ���̰� ū���� �����Ѵ�.
	
	while(vecResultLine.size() > 2)
	{
		float fSum = 0.f;
		for(unsigned int i = 0;i<vecResultLine.size() - 1;++i) {
			float fGrad = (float)(vecResultLine[i + 1].p1.y - vecResultLine[i].p1.y)/(vecResultLine[i + 1].p1.x - vecResultLine[i].p1.x);
			fSum += fGrad;
		}

		//���� ���� ����� ����.
		float fAverage = fSum / (vecResultLine.size() - 1);
		printf("[GRADIENT] [%f] \n",fAverage);

		//��հ� ���� ���� ũ�� ���� �� point�� ���ش�.
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

	//�����Ͱ� 2���� ������ 2���� ���⸦ ���Ѵ�.
	if(vecResultLine.size() == 2) {
		float fGrad = (float)(vecResultLine[1].p1.y - vecResultLine[0].p1.y)/(vecResultLine[1].p1.x - vecResultLine[0].p1.x);
		return fGrad;
	}

	return 0.f;
}