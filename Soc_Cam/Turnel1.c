#include "Turnel1.h"

CTurnel1::CTurnel1(void)	:
iDir(0),
m_iLeft(-1),
m_iRight(-1),
m_bLeft(false),
m_bRight(false),
m_iLine(0)
{
}
CTurnel1::~CTurnel1(void)
{
}

bool CTurnel1::MissionTest(_us (*img)[256]){
	return false;
}
TSEND CTurnel1::Missioning(_us (*img)[256], int now){
	switch(now){
	case 0:
		m_send = Step1(img);
		break;
	case 1:
		m_send = LineChk(img);
		break;
	case 2:
		m_send = Step2(img);
		break;
	}
	return m_send;
}

int CTurnel1::CheckMid(_us (*img)[256]){
	_us r_temp, g_temp, b_temp;
	int i = 0;
	int j = 0;
	int blue_chk = 0;
	int blue_line = 0;
	///////////////왼쪽 카메라
	for(; i < 180; ++i){
		blue_chk = 0;
		for(j = 40; j < 120; ++j)
		{
			r_temp = img[j][i] & _Red;
			g_temp = img[j][i] & _Green;
			b_temp = img[j][i] & _Blue;
			if(r_temp == 0 && g_temp == 0 && b_temp == _Blue){
				++blue_chk;
			}
		}
		if(blue_chk > 40){
			++blue_line;
		}
		else{
			blue_line = 0;
		}
		if(blue_line > 5){
			blue_line = 0;
			return i;
		}
	}
	return -1;
}

TSEND CTurnel1::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;

	if(iDir == 0){
		tsend.state = R_HEAD_LEFT;
	}
	else if(iDir == 1){
		m_iLeft = 180 - CheckMid(img);
		if(m_iLeft == 181)
			m_bLeft = false;
		else
			m_bLeft = true;
		tsend.state = R_HEAD_RIGHT;
	}
	else if(iDir == 2){
		m_iRight = CheckMid(img);
		if(m_iRight == -1)
			m_bRight = false;
		else
			m_bRight = true;
		tsend.state = R_HEAD_CENTER;
	}
	else if(iDir == 3){
		
		///////////////오른쪽 카메라 끝
		tsend.now = MI_NOW;
		tsend.step = MV_0;
		if(m_bLeft && m_bRight){
			if(m_iLeft - m_iRight > 9)
				tsend.state = R_LEFT;
			else if(m_iLeft - m_iRight < -9)
				tsend.state = R_RIGHT;
			else{
				tsend.state = R_TURNRIGHT90;
				tsend.now = MI_NEXT;
			}
		}
		else if(m_bLeft)
			tsend.state = R_LEFT;
		else if(m_bRight)
			tsend.state = R_RIGHT;
		else{
			tsend.state = R_TURNRIGHT90;
			tsend.now = MI_NEXT;
		}
	}
	++iDir;
	if(iDir > 3){
		m_bLeft = false;
		m_bRight = false;
		m_iLeft = -1;
		m_iRight = -1;
		iDir = 0;
	}

	return tsend;
}
void CTurnel1::LineChk2(_us (*img)[256]){
	
	// 기준 값 ------------------------------------------------------------------
	const float fCriterion = 0; // 기준이 되는 기울기 값
	const float fFlat = 0.07; // 허용되는 기준과의 오차 범위
	// step 설정 ----------------------------------------------------------------
	float fResult = GetDegree(img);
	float fValue = fabs(fResult - fCriterion);
	m_temp.step = GetDegreeStep(fValue);
	m_temp.size = 4;
	m_temp.now = MI_NOW;
	if(m_temp.step > MV_3)
		m_temp.step = MV_3;

	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
		m_temp.state = R_TURNLEFT;
	else if(fResult > (fCriterion + fFlat))
		m_temp.state = R_TURNRIGHT;		
	else {
		//안정권안으로 들어옴
		m_temp.state = R_WAIT;
		m_temp.step = MV_0;
		m_temp.now = MI_NEXT;
	}
	////////////////////////////////////////////////

}
TSEND CTurnel1::LineChk(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	if(m_iLine == 0){
		tsend.state = R_LINE_FRONT;
	}
	else if(m_iLine == 1){
		LineChk2(img);
		tsend.state = R_STAND;
	}
	else if(m_iLine == 2){
		tsend = m_temp;
	}
	++m_iLine;
	if(m_iLine > 2)
		m_iLine = 0;
	

	return tsend;
}

TSEND CTurnel1::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_LEFT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	//////////////////////////////////////////////////////////////////////////
	_us r_temp, g_temp, b_temp;
	int i = 0;
	int j = 0;

	int blue_chk = 0;
	int blue_line = 0;	
	int iRight = 0;	
	bool bRight = false;
	static bool bChk = false;	
	///////////////왼쪽 카메라
	for(; i < 180; ++i){
		blue_chk = 0;
		for(j = 0; j < 120; ++j)
		{
			r_temp = img[j][i] & _Red;
			g_temp = img[j][i] & _Green;
			b_temp = img[j][i] & _Blue;
			if(r_temp == 0 && g_temp == 0 && b_temp == _Blue){
				++blue_chk;
			}
		}
		if(blue_chk > 70){
			++blue_line;
		}
		else{
			blue_line = 0;
		}
		if(blue_line > 15){
			blue_line = 0;
			iRight = i;
			bRight = true;
			break;
		}
	}
	if(bRight)
		bChk = true;
	if(iRight > 155 && bChk){
		tsend.state = R_TURNLEFT90;
		tsend.step = MV_0;
		tsend.now = MI_END;

	}
	////////////////////////////////////
	return tsend;
}
