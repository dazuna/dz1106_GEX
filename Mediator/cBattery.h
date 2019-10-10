#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "iMessageInterface.h"

class cBattery : public iMessageInterface
{
public:
	float amountPower;
	glm::vec3 location;
	float Hey;

	// iMessageInterface
	virtual sNVPair RecieveMessage(sNVPair message);
};