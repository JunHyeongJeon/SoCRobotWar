#include "Manager.h"

int main(void)
{
	CManager *pManager = new CManager();

	while(1)
	{
		pManager->Process();
	}

	return 0;
}
