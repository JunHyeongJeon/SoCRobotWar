#include "Bari.h"

///////////////////////////////////////다된듯
CBari::CBari(void)	:
m_ifuck(false)
{	
}


CBari::~CBari(void)
{
}

bool CBari::MissionTest(_us (*img)[256]){

	return false;
}

TSEND CBari::Missioning(_us (*img)[256], int now){
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

TSEND CBari::Step1(_us (*img)[256]){
	
	static int ifuck = 0;

	TSEND tsend;

	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;

	_us r_temp, g_temp, b_temp;
	int i = 20;
	int j = 0;
	int bari_chk;
	int bari_total = 0;
	int first_line = 0;
	int max_pat = 0;
	_us bari_temp = 0xFFFF;
	int max_line = 0;
	int black_chk = 0;
	int yellow_chk = 0;

	for(i = 20; i < 120; ++i){
		bari_chk = -1;
		bari_temp = 0xFFFF;
		black_chk = 0;
		yellow_chk = 0;
		for(j = 0; j < 180; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;

			if(r_temp == _Red && g_temp == _Green && b_temp == 0){
				////TK Yellow Check
				if(bari_chk == -1){
					bari_temp = img[i][j];
					++bari_chk;
					++yellow_chk;
				}
				else{
					if((bari_temp == 0 || bari_temp == _Blue) && black_chk > 8){
						bari_temp = img[i][j];
						++bari_chk;
						black_chk = 0;
					}
					else if(bari_temp == (_Red | _Green)){
						bari_temp = img[i][j];
						++yellow_chk;
						black_chk = 0;
					}
				}
			}
			else if(img[i][j] == 0 || img[i][j] == _Blue){
				////TK Black Check
				if(bari_chk == -1){
					bari_temp = img[i][j];
					++bari_chk;
					++black_chk;
				}
				else{
					if(bari_temp == (_Red | _Green) && yellow_chk > 8){
						bari_temp = img[i][j];
						++bari_chk;
						yellow_chk = 0;
					}
					else if(bari_temp == 0 || bari_temp == _Blue){
						bari_temp = img[i][j];
						++black_chk;
						yellow_chk = 0;
					}
				}
			}

		}// for문 끝

		if(bari_chk >= 3){
			if(!bari_total)
				first_line = i;

			if(bari_chk > max_pat)
				max_pat = bari_chk;
			++bari_total;
			max_line = i;
			
		}
	}

	
	if(max_pat >= 3 && max_pat <=6 && bari_total > 15 && max_line > 65){
		tsend.state = R_SITDOWN;
		tsend.step = MV_0;
		tsend.now = MI_NEXT;
	}
	else if(max_pat > 6 && bari_total > 10){
		tsend.state = R_GO_BARI;
		tsend.step = MV_0;
		tsend.now = MI_NOW;
		printf("---------------  %d    --------------\n", max_line);
	}
	if(!m_ifuck){
		tsend.state = R_GO_BARI;
		tsend.step = MV_0;
		tsend.now = MI_NOW;
		m_ifuck = true;
	}
	return tsend;
}

TSEND CBari::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	//////////////////////////////////////////////////////////////////
	static bool Bari_state = false;
	static bool Bari_go = false;
	
	
	_us r_temp, g_temp, b_temp;
	int i = 20;
	int j = 0;
	int bari_chk;
	int bari_total = 0;
	int first_line = 0;
	int max_pat = 0;
	_us bari_temp = 0xFFFF;
	int max_line = 0;

	int black_chk = 0;
	int yellow_chk = 0;
	for(i = 20; i < 120; ++i){
		bari_chk = -1;
		bari_temp = 0xFFFF;
		black_chk = 0;
		yellow_chk = 0;
		for(j = 0; j < 180; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;

			if(r_temp == _Red && g_temp == _Green && b_temp == 0){
				if(bari_chk == -1){					
					bari_temp = img[i][j];
					++bari_chk;
					++yellow_chk;
				}
				else{
					if((bari_temp == 0 || bari_temp == _Blue)&& black_chk > 10){
						bari_temp = img[i][j];
						++bari_chk;
						black_chk = 0;
					}
					else if(bari_temp == (_Red | _Green)){
						bari_temp = img[i][j];
						++yellow_chk;
						black_chk = 0;
					}
				}


			}
			else if(img[i][j] == 0 || img[i][j] == _Blue){
				if(bari_chk == -1){
					bari_temp = img[i][j];
					++bari_chk;
					++black_chk;
				}
				else{
					if(bari_temp == (_Red | _Green) && yellow_chk > 10){
						bari_temp = img[i][j];
						++bari_chk;
						yellow_chk = 0;
					}
					else if(bari_temp == 0 || bari_temp == _Blue){
						bari_temp = img[i][j];
						++black_chk;
						yellow_chk = 0;
					}
				}
			}

		}// for문 끝
		if(bari_chk >= 2){
			if(!bari_total)
				first_line = i;

			if(bari_chk > max_pat)
				max_pat = bari_chk;
			++bari_total;
			max_line = i;
			
		}
	}


	if(max_pat >= 2 && bari_total > 30 && max_line > 50)
	{
		if(!Bari_state && !Bari_go){
			Bari_state = true;
		}
	}
	else{
		if(Bari_state && !Bari_go){
			tsend.now = MI_NEXT;
			tsend.state = R_GO;
			tsend.step = MV_2;
			Bari_state = false;
			Bari_go = false;
			printf("--------------- kkkkkkkkkkkkkkkkkkkkkkkkkkk --------------\n");
		}
	}
	return tsend;	
}

TSEND CBari::Step3(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_LINE;
	tsend.size = 4;

	return tsend;
}
TSEND CBari::Finish(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_END;
	tsend.size = 4;

	return tsend;
}