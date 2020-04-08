#pragma once
#include <vector>
#include "AI_P5/Sensor.h"

typedef std::map<std::string,Sensor> mapSensors;

struct cVehicle
{
	mapSensors mSensors;
	cGameObject* pVehicleObj;

	cVehicle(cGameObject *theGO);
};
