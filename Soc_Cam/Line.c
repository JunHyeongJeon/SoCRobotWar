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

	//각도 검사
	if(Gradient(img,tsend) == false) {
		//각도가 안정권에 들어오지 않으면 각도 검사만 하고 종료
		return tsend;
	}

	//각도가 안정권에 있을경우 거리 검사
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