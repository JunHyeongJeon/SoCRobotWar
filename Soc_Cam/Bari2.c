#include "Bari2.h"


CBari2::CBari2(void)
{
}


CBari2::~CBari2(void)
{
}

bool CBari2::MissionTest(_us (*img)[256]){	
	return false;
}
TSEND CBari2::Missioning(_us (*img)[256], int now){
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

TSEND CBari2::Step1(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}

TSEND CBari2::Step2(_us (*img)[256]){
	TSEND tsend;
	tsend.state = R_GO;
	tsend.step = MV_0;

	return tsend;
}
