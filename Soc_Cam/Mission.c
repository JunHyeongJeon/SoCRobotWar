#include "Mission.h"

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

float CMission::GetDegree(_us (*img)[256])
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

MV_STEP CMission::GetDegreeStep(float fValue)
{
	return (MV_STEP)((int)fabs(fValue / 0.1924f));
}