#include "LCDDebug.h"

bool IsValidColor(_us checkColor,std::list<_us>& colorList)
{
	if(colorList.empty())
		return true;

	std::list<_us>::iterator it = colorList.begin();
	for(;it != colorList.end();++it)
	{
		_us color = (*it);
		if(checkColor == color)
		{
			return true;
		}
	}

	return false;
}

void GetValidColor(_us (*img)[256],std::list<_us>& colorList)
{
	for(unsigned int i=0;i<120;i++)
	{
		for(int j=0;j<180;++j)
		{
			if(IsValidColor(img[i][j],colorList) == false)
			{
				img[i][j] = _White;
			}
		}
	}
}
//////////TK���� switch�ȿ� MI_�� �ٲ����.
void LCDDebug(_us (*img)[256],M_STATE state)
{
	//printf("----- LDC DEBUG STATE = [%d] ----- \n",state);
	std::list<_us> colorList;

	switch(state)
	{
	//case MI_BARING:
	//case MI_PREBARI:
	//case MI_BARI:
	//case MI_BARINOT:
	//case MI_BARIWAIT:
	//case MI_BARIGO:
	//	//printf("----- COLOR = [BLACK YELLOW] ----- \n");
	//	colorList.push_back(_Black);
	//	colorList.push_back(_Yellow);
	//	break;

	//case MI_PRERED:
	//case MI_REDING:
	//case MI_RED:
	//	//printf("----- COLOR = [RED] ----- \n");
	//	colorList.push_back(_Red);
	//	break;

	//case MI_PREBRIDGE:
	//case MI_BRIDGEING:
	//case MI_BRIDGE:
	//case MI_BRIDGEGO:
	//case MI_BRIDGELEFT:
	//case MI_BRIDGERIGHT:
	//case MI_BRIDGEFINISIH:
	//	//printf("----- COLOR = [BLACK GREEN] ----- \n");
	//	colorList.push_back(_Black);
	//	colorList.push_back(_Green);
	//	break;

	default:
		//printf("----- COLOR = [DEFAULT] ----- \n");
		break;
	}

	GetValidColor(img,colorList);
}

float GetDistance(_us (*img)[256])
{
	std::vector<point> pointList;

	//��� 10���� �˻��Ѵ�.
	for(int x=10;x<180;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				printf("POINT = [%d] [%d] \n",x,y);
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

float GetGradient(_us (*img)[256])
{ 
	std::vector<point> pointList;
	
	//��� ������ point�� ���Ѵ�.
	for(int x=10;x<180;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				printf("POINT = [%d] [%d] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	//point�� 2�� ���ö����� �ݺ��ؼ� PID��� �Ѵ�.
	while(pointList.size() > 2)
	{
		std::vector<float> gradList;
		float fSum = 0;

		//��� ���⸦ ����
		for(unsigned int i=0;i<pointList.size() - 1;++i)
		{
			int nDx = pointList[i + 1].x - pointList[i].x;
			int nDy = pointList[i + 1].y - pointList[i].y;
			nDx == 0 ? nDx = 1 : nDx;

			float fGrad = (float)nDy / nDx;
			gradList.push_back(fGrad);
			fSum += fGrad;
		}

		//���� ���� ����� ����
		float fAverage = fSum / (pointList.size() - 1);

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

		pointList.erase(pointList.begin() + nMaxIndex);
	}

	if(pointList.size() == 2)
	{
		int nDx = pointList[1].x - pointList[0].x;
		int nDy = pointList[1].y - pointList[0].y;
		nDx == 0 ? nDx = 1 : nDx;
		float fGrad = (float)nDy / nDx;

		printf("GRADIENT = [%f] \n",fGrad);
		return fGrad;
	}

	return 0.f;
}
