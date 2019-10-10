#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "commonstuff.h"
#include <string>


//#include "cWorld.h"
//class cWorld;			// forward declaration
//#include "iWorldRobotInterface.h"
#include "iMessageInterface.h"

class cRobot
{
public:

	void setWorldPointer(iMessageInterface* pTheWorld );

	glm::vec3 position;
	glm::vec3 velocity;

	float energy;

	void Update(void);

	void PrintState(void);

	std::string name;

	unsigned int getUniqueID(void);
private:
	unsigned int m_uniqueID;

	iMessageInterface* m_pTheWorld;
};
