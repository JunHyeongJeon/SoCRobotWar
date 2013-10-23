#include "Hurdle.h"


CHurdle::CHurdle(void)	:
m_iChk(0),
m_please(0),
m_kkk(0),
bkkk(false),
m_iSee(0)
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
	case 2:
		m_send = Step3(img);
		break;
	case 3:
		m_send = Finish(img);
		break;
	}
	return m_send;
}

TSEND CHurdle::Step1(_us (*img)[256]){
	/*++m_please;*/
	TSEND tsend;
	tsend.state = R_GO_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	if(m_iSee == 0){
		tsend.state = R_LINE_HURDLE;
		++m_iSee;
	}
	else if(m_iSee == 1){
		_us r_temp, g_temp, b_temp;

		int i = 0;
		int j = 0;
		int blue_line = 0;
		int blue_line2 = 0;
		int blue_chk = 0;
		int blue_chk2 = 0;
		int first_line = 0;
		bool bHurdle = false;
		bool b1 = false;
		static bool bChk = false;
		bool bWhite = false;

		if(!bChk){
			for(i = 0; i < 120; ++i){
				blue_chk = 0;
				blue_chk2 = 0;
				for(j = 10; j < 170; ++j)
				{
					r_temp = img[i][j] & _Red;
					g_temp = img[i][j] & _Green;
					b_temp = img[i][j] & _Blue;
					if(r_temp == 0 && g_temp == 0 && b_temp == _Blue){
						if(!bWhite){
							++blue_chk;						
						}
						else
							++blue_chk2;
					}
				}
				if(blue_chk > 45){
					++blue_line;

				}
				if(blue_line > 1 && !bWhite){
					b1 = true;
					first_line = i - 2;		
					bWhite = true;
					i += 3;
				}
				if(blue_chk2 > 40){
					++blue_line2;
				}
				if(blue_line2 > 1){
					bHurdle = true;
					break;
				}
			}
		}

		printf("HURDLE---------------  %d    --------------\n", first_line);
		if(bHurdle || b1){
			tsend.now = MI_NEXT;
			tsend.step = MV_0;
			tsend.state = R_GO_HURDLE;
			if(first_line < 45)
				tsend.step = MV_1;
			else if(first_line < 60)
				tsend.step = MV_0;
			else if(first_line < 90){
				tsend.step = MV_2;
				tsend.state = R_SHORT_WALK;
			}
		}
		else{
			tsend.now = MI_NEXT;
			tsend.step = MV_0;
			tsend.state = R_GO_HURDLE;
		}
		
	}
	
	/*else if(m_please > 1){
		bkkk = true;
		m_iChk = 0;
		bChk = true;
		if(m_kkk < 4){
			tsend.now = MI_NOW;
			tsend.step = MV_0;
			tsend.state = R_SHORT_WALK;
			++m_kkk;
		}
		else{
			tsend.now = MI_NEXT;
			tsend.step = MV_0;
			tsend.state = R_HURDLE;
		}
	}*/

	return tsend;
}


TSEND CHurdle::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_SHORT_WALK;
	tsend.step = MV_3;
	tsend.now = MI_NEXT;
	tsend.size = 4;

	return tsend;
}
TSEND CHurdle::Step3(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_NEXT;
	tsend.size = 4;

	return tsend;
}
TSEND CHurdle::Finish(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_LINE_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_END;
	tsend.size = 4;

	return tsend;
}