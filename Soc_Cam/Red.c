#include "Red.h"
///// ´ÙµÈµí
CRed::CRed(void)
{
	
}

CRed::~CRed(void)
{
}

bool CRed::MissionTest(_us (*img)[256]){	
	return false;
}

TSEND CRed::Missioning(_us (*img)[256], int now){	
	switch(now){
	case 0:
		m_send = Step1(img);
		break;
	case 1:
		m_send = Step2(img);
		break;
	}
	return m_send;
}
TSEND CRed::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	//////////////////////////////////////////////////
	_us r_temp, g_temp, b_temp;
	int i = 0;
	int j = 0;
	int total_check = 0;
	int red_chk = 0;
	int red_line = 0;
	int max_line = 0;

	for(; i < 120; ++i){
		red_chk = 0;
		for(j = 0; j < 180; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;

			if(r_temp == _Red && g_temp == 0 && b_temp == 0)
				++red_chk;
		}
		if(red_chk > 100){
			max_line = i;
			++red_line;
		}
	}
	
	if(red_line > 7 && max_line > 95){
		tsend.now = MI_END;
		tsend.state = R_CREEP;
		tsend.step = MV_0;
	}
	else if(red_line > 7){
		tsend.state = R_GO;
		tsend.step = MV_2;
	}
	///////////////////////////////

	return tsend;
}
TSEND CRed::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
