#include "Hurdle.h"


CHurdle::CHurdle(void)
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
	tsend.step = MV_0;

	return tsend;
}

TSEND CHurdle::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;
	
	return tsend;
}
