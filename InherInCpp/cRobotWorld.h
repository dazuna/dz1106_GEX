#ifndef _cRobotWorld_HG_
#define _cRobotWorld_HG_

#include "cRobot.h"
#include "iMediatorInterface.h"

#include <vector>

class cRobotWorld : public iMediatorInterface
{
public:
	virtual cFood findClosestGarbage(eGarbageType, float x, float y);
	virtual bool eatGarbage(cFood theGarbage);


	void AddGarbage(void);
	void AddRobotToWorld(cRobot* pTheRobotToAdd);

	std::vector<cFood> vecFood;
private:
	std::vector<cRobot*> m_vec_pRobots;
};

#endif 
