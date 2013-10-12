#include "Turnel1.h"

CTurnel1::CTurnel1(void)	:
iDir(0)
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
		m_send = Step2(img);
		break;
	case 1:
		m_send = Step3(img);
		break;
	case 2:
		m_send = Step3(img);
		break;
	case 3:
		m_send = Step4(img);
		break;
	}
	return m_send;
}

TSEND CTurnel1::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 3;

	_us r_temp, g_temp, b_temp;	
	int i = 0;
	int j = 0;

	int blue_chk = 0;
	int blue_line = 0;
	int iLeft = 0;
	int iRight = 0;
	bool bLeft = false;
	bool bRight = false;

	_us temp = 0xFFFF;
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
		if(blue_chk > 60){
			++blue_line;
		}
		else{
			blue_line = 0;
		}
		if(blue_line > 3){
			if( i < 90 && !bLeft){
				iLeft = i - 2;
				bLeft = true;
				blue_line = 0;
			}
			else if(i >= 90){
				if(!bLeft){				
					iRight = i;
					bRight = true;
				}
				else if(i > iLeft + 100){
					iRight = i;
					bRight = true;
				}
			}
		}
	}

	if(bLeft && bRight){
		tsend.state = R_GO;
		tsend.step = MV_2;
		if((iLeft + iRight) / 2 < 85){
			tsend.state = R_RIGHT;		
		}
		else if((iLeft + iRight) / 2 > 95){
			tsend.state = R_LEFT;
		}
		if(iLeft < 10 && iRight > 170){
			tsend.state = R_GO;
			tsend.now = MI_NEXT;
		}
	}
	else if(bLeft){
		tsend.state = R_RIGHT;
		tsend.step = MV_2;
	}
	else if(bRight){
		tsend.state = R_LEFT;
		tsend.step = MV_2;
	}
	else{
		tsend.now = MI_NEXT;
		tsend.state = R_GO;
		tsend.step = MV_2;
	}

	return tsend;
}
TSEND CTurnel1::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 3;

	if(iDir == 0){
		tsend.state = R_HEAD_LEFT;
	}
	else if(iDir == 1){
		img_left = img;
		tsend.state = R_HEAD_RIGHT;
	}
	else if(iDir == 2){
		img_right = img;
		tsend.state = R_HEAD_CENTER;
	}
	else if(iDir == 3){
		_us r_temp, g_temp, b_temp;
		int i = 0;
		int j = 0;

		int blue_chk = 0;
		int blue_line = 0;
		int iLeft = 0;
<<<<<<< .mine
		int iRight = 0;
=======
		int iRight = 0;		
>>>>>>> .r246
		bool bLeft = false;
		bool bRight = false;		
		///////////////왼쪽 카메라
		for(; i < 180; ++i){
			blue_chk = 0;
			for(j = 0; j < 120; ++j)
			{
				r_temp = img_left[j][i] & _Red;
				g_temp = img_left[j][i] & _Green;
				b_temp = img_left[j][i] & _Blue;
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
			if(blue_line > 7 && !bLeft){
				iLeft = i - 5;
				bLeft = true;
				blue_line = 0;
				break;
			}
		}
		///////////////왼쪽 카메라 끝
		blue_line = 0;
		///////////////오른쪽 카메라 끝
		for(i = 0; i < 180; ++i){
			blue_chk = 0;
			for(j = 0; j < 120; ++j)
			{
				r_temp = img_right[j][i] & _Red;
				g_temp = img_right[j][i] & _Green;
				b_temp = img_right[j][i] & _Blue;
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
			if(blue_line > 7){
				iRight = i;
				bRight = true;						
			}
		}
		///////////////오른쪽 카메라 끝
		tsend.now = MI_NOW;
		tsend.step = MV_2;
		if(bLeft && bRight){		
			if(iLeft - iRight > 30)
				tsend.state = R_RIGHT;
			else if(iLeft - iRight < -30)
				tsend.state = R_LEFT;
			else{
				tsend.state = R_GO;
				tsend.now = MI_END;
			}
		}
		else if(bLeft)
			tsend.state = R_RIGHT;
		else if(bRight)
			tsend.state = R_LEFT;
		else{
			tsend.state = R_TURNLEFT90;
			tsend.now = MI_END;
		}
	}
	++iDir;
	if(iDir > 3)
		iDir = 0;

	return tsend;
}

TSEND CTurnel1::Step3(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_RIGHT;
	tsend.step = MV_2;
	tsend.now = MI_NOW;
	tsend.size = 3;
	//////////////////////////////////////////////////////////////////////////
	_us r_temp, g_temp, b_temp;
	int i = 0;
	int j = 0;

	int blue_chk = 0;
	int blue_line = 0;	
	int iRight = 0;	
	_us temp = 0xFFFF;
	///////////////왼쪽 카메라
	for(; i < 180; ++i){
		blue_chk = 0;
		for(j = 0; j < 120; ++j)
		{
			r_temp = img_left[j][i] & _Red;
			g_temp = img_left[j][i] & _Green;
			b_temp = img_left[j][i] & _Blue;
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
		if(blue_line > 7){
			blue_line = 0;
			iRight = i;
		}
	}
	if(iRight > 160){
		tsend.state = R_TURNLEFT90;
		tsend.step = MV_2;
		tsend.now = MI_END;
	}
	////////////////////////////////////
	return tsend;
}

TSEND CTurnel1::Step4(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 3;

	return tsend;
}
