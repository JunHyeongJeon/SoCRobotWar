#include "Mission.h"

M_STATE CMission::m_state = M_BARI1;
TSEND CMission::m_send;
CMission::CMission(void)
{
}

CMission::~CMission(void)
{
}

bool CMission::MissionTest(_us (*img)[256]){
	return true;
}

TSEND CMission::Missioning(_us (*img)[256], int now){
	return m_send;
}
