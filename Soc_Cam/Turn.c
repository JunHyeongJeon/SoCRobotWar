#include "Turn.h"


CTurn::CTurn(void)	:
m_iLine(0),
m_iLeft(0)
{
}


CTurn::~CTurn(void)
{
}

bool CTurn::MissionTest(_us (*img)[256]){
	return false;
}

TSEND CTurn::Missioning(_us (*img)[256], int now){
	switch(now){
	case 0:
		m_send = Step0(img);
		break;
	case 1:
		m_send = Bottom(img);
		break;
	case 2:
		m_send = Step1(img);
		break;
	case 3:
		m_send = Step2(img);
		break;
	case 4:
		m_send = LineChk(img);
		break;
	case 5:
		m_send = Step3(img);
	}
	return m_send;
}

TSEND CTurn::Step0(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO_HURDLE;
	tsend.step = MV_2;
	tsend.now = MI_LINE;
	tsend.size = 4;
	return tsend;
}
TSEND CTurn::Bottom(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_LINE_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_NEXT;
	tsend.size = 4;
	return tsend;
}
TSEND CTurn::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	_us r_temp, g_temp, b_temp;
	_us temp = 0xFFFF;
	int i = 119;
	int j = 0;
	int black_chk = 0;
	int black_line = 0;
	int black_chk2 = 0;
	int black_con = 0;
	int first_line = 0;


	for(; i > 75; --i){
		black_chk = 0;
		for(j = 0; j < 180; ++j)
		{
			if(j != 0)
				temp = img[i][j-1]; 

			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;
			if(img[i][j] == 0 || img[i][j] == _Blue){
				++black_chk;
				if(temp == 0 || temp == _Blue)
					++black_chk2;
			}
			if(black_chk2 > 9)
				++black_con;
		}
		if(black_chk > 60 && black_con > 1)
			++black_line;
		if(black_line > 4){
			first_line = i- 3;
			break;
		}

	}

	if(first_line > 83){
		tsend.state = R_LEFT;
		tsend.step = MV_0;
		tsend.now = MI_NEXT;
	}
	return tsend;
}

void CTurn::LineChk2(_us (*img)[256]){

	// 기준 값 ------------------------------------------------------------------
	const float fCriterion = 0; // 기준이 되는 기울기 값
	const float fFlat = 0.07; // 허용되는 기준과의 오차 범위
	// step 설정 ----------------------------------------------------------------
	float fResult = GetDegree(img, 0x00);
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
		m_temp.state = R_TURNLEFT90;
		m_temp.step = MV_0;
		m_temp.now = MI_LINE;
	}
	////////////////////////////////////////////////

}

TSEND CTurn::LineChk(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	if(m_iLine == 0){
		tsend.state = R_LINE_FRONT1;
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


TSEND CTurn::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.now = MI_NEXT;
	tsend.state = R_LEFT;
	tsend.step = MV_0;
	tsend.size = 4;

	return tsend;
}

TSEND CTurn::Step3(_us (*img)[256]){
	TSEND tsend;
	tsend.now = MI_END;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.size = 4;
	return tsend;
}