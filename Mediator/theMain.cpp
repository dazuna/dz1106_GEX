#include "cWorld.h"
#include "cObserver.h"
#include <iostream>

int main()
{

	cWorld* pWorld = new cWorld();

	pWorld->GenerateWorld( 10, 10, 500.0f );


	void setRobot(cRobot * pTheRobot);
	void Update(void);

	cObserver* pObs = new cObserver;
	pObs->setRobot( pWorld->m_vec_pRobots[0] );

	while (true)
	{
		pObs->Update();

		pWorld->m_vec_pRobots[0]->Update();
	}







	return 0;
}