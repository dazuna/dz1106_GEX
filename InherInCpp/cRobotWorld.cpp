#include "cRobotWorld.h"

void cRobotWorld::AddRobotToWorld(cRobot* pTheRobotToAdd)
{
	this->m_vec_pRobots.push_back(pTheRobotToAdd);
}


void cRobotWorld::AddGarbage(void)
{
	// Pick a random location for the garbage

	cFood newGarbage;
	newGarbage.type = ALUMINUM;
	newGarbage.amount = 1.0f;
	newGarbage.x = 50.0f;
	newGarbage.y = 100.0f;

	this->vecFood.push_back(newGarbage);


}


cFood cRobotWorld::findClosestGarbage(eGarbageType, float x, float y)
{
	cFood closestFood;

	if ( ! this->vecFood.empty() )
	{
		closestFood = this->vecFood[0];
	}
	return closestFood;
}

bool cRobotWorld::eatGarbage(cFood theGarbage)
{

	return true;
}

