#include "Turnel1.h"

CTurnel1::CTurnel1(void)	:
iDir(0),
m_iLeft(-1),
m_iRight(-1),
m_bLeft(false),
m_bRight(false),
m_iLine(0),
m_bCenter(false),
m_bGo(false)
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
	///////////////���� ī�޶�
	for(; i < 180; ++i){
		blue_chk = 0;
		for(j = 10; j < 120; ++j)
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

TSEND CTurnel1::ChkCenter(_us (*img)[256]){
	TSEND tsend;
	tsend.step = MV_0;
	tsend.state = R_TURNRIGHT90;
	tsend.now = MI_NEXT;
	tsend.size = 4;

	_us r_temp, g_temp, b_temp;
	int i = 0;
	int j = 0;
	int max_line = 0;

	int blue_chk = 0;
	int blue_line = 0;	
	int iLeft = -1;
	int iRight = -1;	
	///////////////���� ī�޶�
	for(i = 0; i < 30; ++i){
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
		if(blue_chk > 50)
			++blue_line;
		else
			blue_line = 0;
		if(blue_line > 2){
			iLeft = i - 2;
			break;
		}
	}
	blue_line = 0;
	for(i = 160; i < 180; ++i){
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
		if(blue_chk > 50)
			++blue_line;
		else
			blue_line = 0;
		if(blue_line > 2){
			iRight = i;
			break;
		}
	}
	if(iLeft != -1 && iRight != -1){
		if(iLeft < 7 && iRight > 173){
			m_bCenter = true;
			return tsend;
		}

	}
	if(iLeft != -1 && iRight != -1){
		if(iLeft < 7 && iRight > 173){
			m_bCenter = true;
			return tsend;
		}

	}
	blue_line = 0;
	for(i = 0; i < 180; ++i){
		blue_chk = 0;
		for(j = 0; j < 50; ++j)
		{
			r_temp = img[i][j] & _Red;
			g_temp = img[i][j] & _Green;
			b_temp = img[i][j] & _Blue;
			if(r_temp == 0 && g_temp == 0 && b_temp == _Blue){
				++blue_chk;
			}			
		}
		if(blue_chk > 50){
			++blue_line;
			max_line = i;
		}
	}
	m_bCenter = false;
	if(blue_line > 2 && max_line > 15){
		tsend.now = MI_NOW;
		tsend.state = R_GO;
		tsend.step = MV_0;
		m_bGo = true;
		return tsend;
	}
	m_bGo = false;
	
	return tsend;

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
		
		///////////////������ ī�޶� ��
		tsend = ChkCenter(img);
		if(m_bCenter || m_bGo){
			m_bCenter = false;
			m_bLeft = false;
			m_bRight = false;
			m_bGo = false;
			m_iLeft = -1;
			m_iRight = -1;
			iDir = 0;
			return tsend;
		}
		tsend.now = MI_NOW;
		tsend.step = MV_0;

		if(m_bLeft && m_bRight){
			if(m_iLeft - m_iRight > 4)
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
	
	// ���� �� ------------------------------------------------------------------
	const float fCriterion = 0; // ������ �Ǵ� ���� ��
	const float fFlat = 0.07; // ���Ǵ� ���ذ��� ���� ����
	// step ���� ----------------------------------------------------------------
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
		//�����Ǿ����� ����
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
	///////////////���� ī�޶�
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
