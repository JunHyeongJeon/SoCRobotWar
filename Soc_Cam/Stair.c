#include "Stair.h"


CStair::CStair(void)	:
m_iLine(0),
m_ishort(0),
m_preUp(0),
m_stWalk(0),
iMid(0),
m_iUp(0),
m_iGo(0),
m_bGoing(false)
{
}


CStair::~CStair(void)
{
}

bool CStair::MissionTest(_us (*img)[256]){
	return false;
}
TSEND CStair::Missioning(_us (*img)[256], int now){
	switch(now){
	case 0:
		m_send = Up1(img);
		break;
	case 1:
		m_send = Up2(img);
		break;
	case 2:
		m_send = Up3(img);
		break;
	case 3:
		m_send = Step4(img);
		break;
	case 4:
		m_send = Step5(img);
		break;
		/////////////////////////////////// 올라가기전
	case 5:
		m_send = Up1(img);
		break;
	case 6:
		m_send = Up2(img);
		break;
	}
	return m_send;
}
TSEND CStair::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_LINE;
	tsend.size = 4;
	/*if(m_iLine == 0){
		tsend.state = R_LINE_FRONT0;
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

*/
	return tsend;
}

TSEND CStair::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO_HURDLE;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;

	if(m_iLine == 0){
		tsend.state = R_LINE_HURDLE;
	}
	else if(m_iLine == 1){
		_us r_temp, g_temp, b_temp;
		int i = 0;
		int j = 0;
		int green_chk = 0;
		int green_line = 0;
		int first_line = 0;
		for(i = 0; i < 120; ++i){
			green_chk = 0;
			for(j = 0; j < 180; ++j)
			{
				r_temp = img[i][j] & _Red;
				g_temp = img[i][j] & _Green;
				b_temp = img[i][j] & _Blue;
				if(r_temp == 0 && g_temp == _Green && b_temp == 0){
					++green_chk;
				}
			}
			if(green_chk > 15){
				++green_line;
				first_line = i;
			}
		}

		if(first_line < 95){
			tsend.state = R_GO_HURDLE;
			tsend.step = MV_0;
			tsend.now = MI_NEXT;
		}
		else{
			tsend.state = R_SHORT_WALK;
			tsend.step = MV_2;
			tsend.now = MI_NEXT;
		}
	}
	++m_iLine;
	if(m_iLine > 1)
		m_iLine = 0;

	return tsend;
}

TSEND CStair::Step3(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_SHORT_WALK;
	tsend.step = MV_4;
	tsend.now = MI_NEXT;
	tsend.size = 4;
	return tsend;
}

TSEND CStair::Step4(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	if(m_preUp== 0){
		tsend.state = R_LINE_FRONT0;
	}
	else if(m_preUp == 1){
		iMid = PreUpMid(img);
		if(iMid < 85){
			m_temp = tsend;
			m_temp.state = R_LEFT;
		}
		else if(iMid > 93){
			m_temp = tsend;
			m_temp.state = R_RIGHT;
		}
		else{
			m_temp = tsend;
			m_temp.state = R_SHORT_WALK;
			m_temp.now = MI_NEXT;
		}
		tsend.state = R_STAND;
	}
	else if(m_preUp == 2){
		tsend = m_temp;
	}
	++m_preUp;
	if(m_preUp > 2){
		m_preUp = 0;
		iMid = 0;
	}

	return tsend;
}

TSEND CStair::Step5(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_STAIR_UP;
	tsend.step = MV_0;
	tsend.now = MI_NEXT;
	tsend.size = 4;
	return tsend;
}


int CStair::PreUpMid(_us (*img)[256]){

	_us r_temp, g_temp, b_temp;
	m_list.clear();
	int i = 119;
	int j = 0;
	int ilimit = -1;
	int green_chk = 0;
	int green_line = 0;
	int first_line = 0;
	bool bChk =false;
	bool bFirst = false;
	int iTemp1 = 0;
	int iTemp2 = 0;
	for(; i > ilimit; --i){
		green_chk = 0;
		iTemp1 = -1;
		iTemp2 = -1;
		for(j = 4; j < 175; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;
			if(r_temp == 0 && g_temp == _Green && b_temp == 0){
				if(!bFirst){
					ilimit = i - 90;
					if(ilimit < -1)
						ilimit = -1;
					bFirst = true;
				}
				if(!bChk){
					bChk = true;
					iTemp1 = j;
				}
				else
					iTemp2 = j;
			}
		}
		bChk = false;
		if(iTemp2 == -1 || iTemp1 == -1){

		}
		else
			m_list.push_back((iTemp1 + iTemp2) / 2);
	}
	int inum = 0;
	int iAver = 0;
	for(std::list<int>::iterator iter = m_list.begin();
		iter != m_list.end(); ++iter){
			++inum;
			if(*iter == 0){
				--inum;
			}
			else
				iAver += *iter;
	}
	return iAver / inum;
	////////////////////////////////////////////////

}


bool CStair::ChkMid(_us (*img)[256]){
	_us r_temp, g_temp, b_temp;
	m_Llist.clear();
	m_Rlist.clear();
	int i = 119;
	int j = 0;
	int ilimit = -1;
	int green_chk = 0;
	int green_line = 0;
	int first_line = 0;
	bool bChk =false;
	bool bFirst = false;
	int iTemp1 = 0;
	int iTemp2 = 0;
	for(; i > ilimit; --i){
		green_chk = 0;
		iTemp1 = -1;
		iTemp2 = -1;
		for(j = 4; j < 175; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;
			if(r_temp == 0 && g_temp == _Green && b_temp == 0){
				if(!bFirst){
					ilimit = i - ((i / 10) * 10);
					if(ilimit < -1)
						ilimit = -1;
					bFirst = true;
				}
				if(!bChk){
					iTemp1 = j;
					bChk = true;
				}
				else
					iTemp2 = j;
			}
		}		
		bChk = false;
		if(iTemp2 == -1 || iTemp1 == -1){

		}
		else{
			m_Llist.push_back(iTemp1);
			m_Rlist.push_back(iTemp2);
		}
	}

	std::list<int>::iterator iterR = m_Rlist.begin();
	int RMax = -1;
	int RMin = -1;
	int LMax = -1;
	int LMin = -1;
	RMax = *iterR;
	++iterR;
	for(int k = 0; k < 3; ++k){
		if(*iterR > RMax)
			RMax = *iterR;
		++iterR;
		if(iterR == m_Rlist.end())
			break;
	}
	iterR = m_Rlist.end();
	--iterR;
	RMin = *iterR;
	--iterR;
	for(int k = 0; k < 3; ++k){
		if(*iterR < RMin)
			RMin = *iterR;
		--iterR;
		if(iterR == m_Rlist.begin())
			break;
	}



	std::list<int>::iterator iterL = m_Llist.begin();
	LMin = *iterL;
	++iterL;
	for(int k = 0; k < 3; ++k){
		if(*iterL < LMin)
			LMin = *iterL;
		++iterL;
		if(iterL == m_Llist.end())
			break;
	}
	iterL = m_Llist.end();
	--iterL;
	LMax = *iterL;
	--iterL;
	for(int k = 0; k < 3; ++k){
		if(*iterL > LMax)
			LMax = *iterL;
		--iterL;
		if(iterL == m_Llist.begin())
			break;
	}
	m_Utemp.now = MI_NOW;
	m_Utemp.size = 4;
	m_Utemp.step = MV_0;

	printf("----------------------------------------------------------\n");
	printf("----------------------------------------------------------\n");
	printf("----------------------------------------------------------\n");
	printf("----------------------------------------------------------\n");
	printf("\t%d\t\t%d\n", LMin, LMax);
	printf("\t%d\t\t%d\n", RMin, RMax);
	if(RMax - 38 > RMin){
		m_Utemp.state = R_TURNLEFT;		
	}
	else if(LMax - 42 > LMin){
		m_Utemp.state = R_TURNRIGHT;
	}
	else{
		m_Utemp.state = R_WAIT;
		return true;
	}
	return false;
}

TSEND CStair::Up1(_us (*img)[256]){
	TSEND tsend;
	tsend.now = MI_NOW;
	tsend.size = 4;
	tsend.step = MV_0;
	tsend.state = R_WAIT;

	if(m_stWalk == 0){
		tsend.state = R_LINE_FRONT1;
	}
	else if(m_stWalk == 1){
		if(ChkMid(img)){
			iMid = PreUpMid(img);
			if(iMid < 80){
				m_Utemp = tsend;
				m_Utemp.state = R_LEFT;
			}
			else if(iMid > 98){
				m_Utemp = tsend;
				m_Utemp.state = R_RIGHT;
			}
			else{
				m_Utemp = tsend;
				m_Utemp.state = R_GO_HURDLE;
				m_Utemp.now = MI_NOW;
			}
		}		
		tsend.state = R_STAND;
	}
	else if(m_stWalk == 2){
		tsend = m_Utemp;
	}
	if(tsend.state == R_GO_HURDLE){
		m_bGoing = true;
	}

	if(m_bGoing){
		tsend.state = R_GO_HURDLE;
		if(m_stWalk == 1){
			m_bGoing = false;
			m_stWalk = 0;
		}
		
	}
	
	if(tsend.state == R_GO_HURDLE)
		++m_iGo;
	if(m_iGo > 2){
		tsend.now = MI_NEXT;
		m_stWalk = 0;
		m_iGo = 0;
		m_bGoing = false;
	}

	
	++m_stWalk;
	if(m_stWalk > 2)
		m_stWalk = 0;


	return tsend;

}

TSEND CStair::Up2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_WAIT;
	tsend.step = MV_0;
	tsend.now = MI_NOW;
	tsend.size = 4;
	if(m_stWalk == 0){
			tsend.state = R_LINE_FRONT1;
		
	}
	else if(m_stWalk == 1){
		if(LineChk2(img)){
			iMid = PreUpMid(img);
			if(iMid < 80){
				m_Utemp = tsend;
				m_Utemp.state = R_LEFT;
			}
			else if(iMid > 98){
				m_Utemp = tsend;
				m_Utemp.state = R_RIGHT;
			}
			else{
				m_Utemp = tsend;
				m_Utemp.state = R_GO_HURDLE;
				m_Utemp.now = MI_NOW;
			}
		}		
		tsend.state = R_STAND;
	}
	else if(m_stWalk == 2){
		tsend = m_Utemp;
	}

	if(tsend.state == R_GO_HURDLE){
		m_bGoing = true;
	}

	if(m_bGoing){
		tsend.state = R_GO_HURDLE;
		if(m_stWalk == 0){
			m_bGoing = false;
			m_stWalk = 0;
		}

	}

	if(tsend.state == R_GO_HURDLE)
		++m_iGo;
	if(m_iGo > 2){
		tsend.now = MI_NEXT;
		tsend.state = R_LINE_FRONT1;
		m_stWalk = 0;
		m_iGo = 0;
		m_bGoing = false;
	}

	++m_stWalk;
	if(m_stWalk > 2)
		m_stWalk = 0;


	return tsend;
}
TSEND CStair::Up3(_us (*img)[256]){
	TSEND tsend;
	tsend.now = MI_NEXT;
	tsend.size = 4;
	tsend.step = MV_0;
	tsend.state = R_CREEP;
	return tsend;
}
TSEND CStair::Up4(_us (*img)[256]){
	TSEND tsend;
	tsend.now = MI_NOW;
	tsend.size = 4;
	tsend.step = MV_0;
	tsend.state = R_WAIT;
	return tsend;
}

bool CStair::LineChk2(_us (*img)[256]){

	// 기준 값 ------------------------------------------------------------------
	const float fCriterion = 0; // 기준이 되는 기울기 값
	const float fFlat = 0.07; // 허용되는 기준과의 오차 범위
	// step 설정 ----------------------------------------------------------------
	float fResult = GetDegree(img, 0x00);
	float fValue = fabs(fResult - fCriterion);
	m_Utemp.step = GetDegreeStep(fValue);
	m_Utemp.size = 4;
	m_Utemp.now = MI_NOW;
	if(m_Utemp.step > MV_3)
		m_Utemp.step = MV_3;
	// --------------------------------------------------------------------------
	if(fResult < (fCriterion - fFlat))
		m_Utemp.state = R_TURNLEFT;
	else if(fResult > (fCriterion + fFlat))
		m_Utemp.state = R_TURNRIGHT;
	else {
		//안정권안으로 들어옴
		m_Utemp.state = R_TURNLEFT90;
		m_Utemp.step = MV_0;
		m_Utemp.now = MI_LINE;
		return true;
	}
	return false;
	////////////////////////////////////////////////

}