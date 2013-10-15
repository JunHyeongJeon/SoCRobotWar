#include "Hurdle.h"


CHurdle::CHurdle(void)	:
m_iChk(0)
{
}


CHurdle::~CHurdle(void)
{
}

bool CHurdle::MissionTest(_us (*img)[256]){
	return false;
}
TSEND CHurdle::Missioning(_us (*img)[256], int now){
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

TSEND CHurdle::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_2;
	tsend.now = MI_NOW;
	int i = 0;
	_us r_temp, g_temp, b_temp;
	int j = 0;
	int blue_line = 0;
	int blue_line2 = 0;
	int blue_chk = 0;
	int blue_chk2 = 0;
	int white_chk = 0;
	int white_line = 0;
	int first_line = 0;
	bool bHurdle = false;
	bool bChk = false;
	
	if(!bChk){
		for(i = 30; i < 120; ++i){
			blue_chk = 0;
			white_chk = 0;
			for(j = 0; j < 180; ++j)
			{
				r_temp = img[i][j] & _Red;
				g_temp = img[i][j] & _Green;
				b_temp = img[i][j] & _Blue;
				if(r_temp == 0 && g_temp == 0 && b_temp == _Blue){
					if(white_chk < 5){
						++blue_chk;
						first_line = i;
					}
					else
						++blue_chk2;
				}
				else if(img[j][i] == 1 && blue_line > 2){
					++white_chk;
				}
			}
			if(blue_chk > 80){
				++blue_line;
			}
			if(white_chk > 80){
				++white_line;
			}
			if(blue_chk2 > 80){
				++blue_line2;
			}
			if(blue_line2 > 2){
				bHurdle = true;
				break;
			}
		}
	}
	
	if(bHurdle){
		tsend.now = MI_NOW;
		tsend.step = MV_2;
		tsend.state = R_GO;
		if(first_line > 40){
			bChk = true;
			bHurdle = false;
		}		
	}
	else if(bChk){
		if(m_iChk < 2){
			tsend.now = MI_NOW;
			tsend.state = R_GO;
			tsend.step = MV_2;
			++m_iChk;
		}
		else{
			tsend.now = MI_END;
			tsend.step = MV_0;
			//tsend.state = ±¸¸£±â;
		}		
	}

	return tsend;
}

TSEND CHurdle::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;
	
	return tsend;
}
