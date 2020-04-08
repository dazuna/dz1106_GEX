#include "cVehicle.h"
#include "GameTools.h"

cVehicle::cVehicle(cGameObject* theGO)
{
	pVehicleObj = theGO;
	auto dstBtwn = theGO->scale * GameTools::worldScale;
	mSensors.insert({"veryLeft",	Sensor(theGO,dstBtwn,60)});	
	mSensors.insert({"Left",		Sensor(theGO,dstBtwn,30)});	
	mSensors.insert({"Center",		Sensor(theGO,dstBtwn,0)});	
	mSensors.insert({"Right",		Sensor(theGO,dstBtwn,-30)});	
	mSensors.insert({"veryRight",	Sensor(theGO,dstBtwn,-60)});	
}

