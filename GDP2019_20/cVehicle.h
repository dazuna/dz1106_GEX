#pragma once
#include <vector>
#include "AI_P5/Sensor.h"

typedef std::map<std::string,Sensor> mapSensors;
typedef std::map<std::string,float> mapFloat;

struct cVehicle
{
	mapSensors mSensors;
	mapFloat mFuzzyValues;
	cGameObject* pVehicleObj;

	cVehicle(cGameObject *theGO);
	void update(float);
};
