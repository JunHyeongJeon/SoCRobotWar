#include "Turnel2.h"


CTurnel2::CTurnel2(void)
{
}


CTurnel2::~CTurnel2(void)
{
}

bool CTurnel2::MissionTest(_us (*img)[256]){
	return false;
}
TSEND CTurnel2::Missioning(_us (*img)[256], int now){
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
TSEND CTurnel2::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
TSEND CTurnel2::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
