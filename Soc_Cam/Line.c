#include "Line.h"
#define GRADIENT_SAFETY_ZONE 0.1924
#define DISTANCE_SAFETY_ZONE 9
#define GRADIENT_SAFETY_CRITERION 0
#define DISTANCE_SAFETY_CRITERION 90



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
	float fResult = GetGradient(img);
	float fValue = fabs(fResult - GRADIENT_SAFETY_CRITERION);

	tsend.step = GetGradientStep(fValue); tsend.size = 4;

	printf("[SOC CAM] [ GRADIENT ] [RESULT = [%f]] \n",fResult);
	if(fResult < (GRADIENT_SAFETY_CRITERION - GRADIENT_SAFETY_ZONE))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_TURNLEFT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (GRADIENT_SAFETY_CRITERION + GRADIENT_SAFETY_ZONE))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_TURNRIGHT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//�����Ǿ����� ����
		tsend.state = R_WAIT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ GRADIENT ] [R_WAIT] [SUCCESS] \n");
		return true;
	}
}

bool CLine::Distance(_us (*img)[256],TSEND &tsend)
{
	float fResult = GetDistance(img);
	float fValue = fabs(fResult - DISTANCE_SAFETY_CRITERION);

	tsend.step = GetDistanceStep(fValue); tsend.size = 4;
	printf("[SOC CAM] [ DISTANCE ] [RESULT = [%f]] \n",fResult);

	if(fResult < (GRADIENT_SAFETY_CRITERION - DISTANCE_SAFETY_ZONE))
	{
		//������ ���� �Ÿ� ���� ���� ���� �������
		tsend.state = R_RIGHT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_RIGHT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else if(fResult > (GRADIENT_SAFETY_CRITERION + DISTANCE_SAFETY_ZONE))
	{
		//������ ���� �Ÿ� ���� ���� ���� Ŭ���
		tsend.state = R_LEFT; tsend.now = (MI_COMP)MI_LINE_NOW;
		printf("[SOC CAM] [ DISTANCE ] [R_LEFT] [STEP = [%d]] \n",tsend.step);
		return false;
	}
	else {
		//�����Ǿ����� ����
		tsend.state = R_WAIT; tsend.now = (MI_COMP)MI_LINE_END;
		printf("[SOC CAM] ---------- [ DISTANCE ] [R_WAIT] [ SUCCESS ] ---------- \n");
		return true;
	}
}

MV_STEP CLine::GetDistanceStep(float fValue)
{
	int nCount = (int)fabs(fValue / 9);
	if(nCount > 5) {
		return (MV_STEP)5;
	}

	return (MV_STEP)nCount;
}

MV_STEP CLine::GetGradientStep(float fValue)
{
	int nCount = (int)fabs(fValue / 0.1924f);
	if(nCount > 5) {
		return (MV_STEP)8;
	}

	return (MV_STEP)nCount;
}

std::vector<LINE> CLine::GetPoint(_us (*img)[256])
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
					//printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
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

	//printf("PROCESS DUPLICATE POINT\n");
	for(unsigned int i = 0;i < vecResultLine.size();++i)
	{
		point p1 = vecResultLine[i].p1;
		point p2 = vecResultLine[i].p2;
		//printf("[LINE] [(%d , %d)] [(%d , %d)] [%d] \n",p1.x,p1.y,p2.x,p2.y,p1.y - p2.y);
	}

	return vecResultLine;
}

void CLine::doStandardiZation(std::vector<LINE>& vecLine,int nPointCount)
{
	//�����Ͱ� nPointCount��ŭ ������ ���� ���� ���̰� ū���� �����Ѵ�.
	while(vecLine.size() > nPointCount)
	{
		float fSum = 0.f;
		for(unsigned int i = 0;i<vecLine.size() - 1;++i) {
			float fGrad = (float)(vecLine[i + 1].p1.y - vecLine[i].p1.y)/(vecLine[i + 1].p1.x - vecLine[i].p1.x);
			fSum += fGrad;
		}

		//���� ���� ����� ����.
		float fAverage = fSum / (vecLine.size() - 1);
		//printf("[GRADIENT] [%f] \n",fAverage);

		//��հ� ���� ���� ũ�� ���� �� point�� ���ش�.
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


float CLine::GetDistance(_us (*img)[256])
{
	printf(" DISTANCE \n");
	std::vector<LINE> vecLine = GetPoint(img);

	//�˻��� �����Ͱ� ���ų� 1���� ������� 0�� ��ȯ
	if(vecLine.size() == 0) {
		return 0.f;
	}

	if(vecLine.size() == 1) {
		return vecLine[0].p1.y;
	}

	printf(" DISTANCE STANDARD ZATION \n");
	doStandardiZation(vecLine,2);

	//�����Ͱ� 2���� ������ 2���� ��� ���̸� ���Ѵ�.
	if(vecLine.size() == 2) {
		float fLength = (vecLine[0].p1.y + vecLine[1].p1.y) / 2.f;
		return fLength;
	}

	printf(" DISTANCE RETURN 0 \n");
	return 0.f;
}

float CLine::GetGradient(_us (*img)[256])
{ 
	std::vector<LINE> vecLine = GetPoint(img);

	//�˻��� �����Ͱ� ���ų� 1���� ������� 0�� ��ȯ
	if(vecLine.size() == 0 || vecLine.size() == 1) {
		return 0.f;
	}

	doStandardiZation(vecLine,2);

	//�����Ͱ� 2���� ������ 2���� ���⸦ ���Ѵ�.
	if(vecLine.size() == 2) {
		float fGrad = (float)(vecLine[1].p1.y - vecLine[0].p1.y)/(vecLine[1].p1.x - vecLine[0].p1.x);
		return fGrad;
	}

	return 0.f;
}