#pragma once

#include "commonstuff.h"
#include "cRobot.h"
#include "cBattery.h"
#include "cHuman.h"
#include <vector>

#include "iWorldRobotInterface.h"

class cWorld : 
	//public iWorldRobotInterface, 
	public iMessageInterface
{
public:
	// DON'T want the robots to call
	void GenerateWorld(int numRobots, int numBatteries, float worldSize);

	//// from iWorldRobotInterface
	virtual float getWorldSize(void);
	virtual cBattery* findLargestBattery(); 

	// from iMessageInterface
	virtual sNVPair RecieveMessage(sNVPair message);


	std::vector<cBattery*> m_vec_pBatteries;
	std::vector<cRobot*> m_vec_pRobots;
	std::vector<cHuman*> m_vec_pHumans;

private:
	float m_worldSize;

};