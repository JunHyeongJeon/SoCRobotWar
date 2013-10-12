#include "Stair.h"


CStair::CStair(void)
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
		Step1(img);
		break;
	case 1:
		Step2(img);
		break;
	}
	return m_send;
}
TSEND CStair::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
TSEND CStair::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
