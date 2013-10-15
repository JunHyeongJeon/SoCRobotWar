#include "Turn.h"


CTurn::CTurn(void)
{
}


CTurn::~CTurn(void)
{
}

TSEND CTurn::Missioning(_us (*img)[256], int now){
	switch(now){
	case 0:
		m_send = Step1(img);
		break;	
	}
}

TSEND CTurn::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO_BARI;
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


	for(; i > -1; --i){
		black_chk = 0;
		for(j = 0; j < 180; ++j)
		{
			if(j != 0)
				temp = img[i][j-1]; 
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;
			if(img[i][j] == 0){
				++black_chk;
				if(temp == 0)
					++black_chk2;
			}
			if(black_chk2 > 9)
				++black_con;
		}
		if(black_chk > 50 && black_con > 3)
			++black_line;
		if(black_line > 3){
			first_line = i- 3;
			break;
		}

	}

	if(i > 80){
		tsend.state = R_TURNLEFT90;
		tsend.step = MV_0;
		tsend.now = MI_NEXT;
	}
}