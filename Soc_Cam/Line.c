#include "Line.h"

CLine::CLine()	:
RRepeat(false),
LRepeat(false),
LChk(0),
RChk(0)
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
	if(!RRepeat && !LRepeat){
		if(tsend.state == R_LEFT)		LRepeat = true;
		else if(tsend.state == R_RIGHT) RRepeat = true;
	}
	else if(RRepeat){
		++RChk;
		if(RChk == 1 && tsend.state == R_LEFT){}
		else if(RChk == 2 && tsend.state == R_RIGHT){
			tsend.now = (MI_COMP)MI_LINE_END;
			tsend.state = R_WAIT;	}
		else{
			RRepeat = false;
			RChk = 0;
		}
	}
	else if(LRepeat){
		++LChk;
		if(LChk == 1 && tsend.state == R_RIGHT){}
		else if(LChk == 2 && tsend.state == R_LEFT){
			tsend.now = (MI_COMP)MI_LINE_END;
			tsend.state = R_WAIT;	}
		else{
			LRepeat = false;
			LChk = 0;
		}
	}
	if(tsend.now == (MI_COMP)MI_LINE_END)	RChk = LChk = 0;
	
	return tsend;
}