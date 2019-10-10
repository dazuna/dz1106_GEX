#ifndef _cRobot_HG_
#define _cRobot_HG_

//#include "cRobotWorld.h"
#include "iMediatorInterface.h"

class cRobot
{
public:
	void FindFood(void);

	//void SetWorldPointer( cRobotWorld* pTheWorld );
	void SetWorldPointer( iMediatorInterface* pTheWorld );

private:
	//cRobotWorld* m_pTheWorld;
	iMediatorInterface* m_pTheWorld;
};

#endif
