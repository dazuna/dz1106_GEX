#include "glm/gtx/rotate_vector.hpp"
#include "cVehicle.h"
#include "GameTools.h"

cVehicle::cVehicle(cGameObject* theGO)
{
	pVehicleObj = theGO;
	// The cone model length is 2
	auto dstBtwn = theGO->scale * 3;
	mSensors.insert({"veryLeft",	Sensor(theGO,dstBtwn,60)});	
	mSensors.insert({"Left",		Sensor(theGO,dstBtwn,30)});	
	mSensors.insert({"Center",		Sensor(theGO,dstBtwn,0)});	
	mSensors.insert({"Right",		Sensor(theGO,dstBtwn,-30)});	
	mSensors.insert({"veryRight",	Sensor(theGO,dstBtwn,-60)});	
}

void cVehicle::update(float dt)
{
	float suma = 0.f, leftForce = 0.f;
	for (auto sensor: mSensors)
	{
		auto fuzzyValue = sensor.second.getCollisionFuzzyValue();
		mFuzzyValues.insert({sensor.first,fuzzyValue});
		suma += fuzzyValue;
		if( sensor.first == "Left" || sensor.first == "veryLeft" ) {leftForce += fuzzyValue;}
		if( sensor.first == "Right" || sensor.first == "veryRight" ) {leftForce -= fuzzyValue;}	
	}
	// 15 is vehicle default speed
	auto maxSpeed = (suma / 5) * 15.f;
	if(suma == 5.f)
	{
		auto dir = glm::normalize(pVehicleObj->velocity);
		pVehicleObj->velocity = dir * maxSpeed;
		return;
	}
	if(fabs(leftForce) < 0.001f) {leftForce = 2.f;}

	if (mFuzzyValues["Center"] < 1.f)
	{
		leftForce *= (1 - mFuzzyValues["Center"]) * 3.f;
	}
	
	auto desiredVelocity = this->pVehicleObj->velocity;
	desiredVelocity = glm::rotate(desiredVelocity,glm::radians(leftForce*30),glm::vec3(0,1,0));
	auto steeringForce = desiredVelocity-this->pVehicleObj->velocity;
	pVehicleObj->velocity += steeringForce * dt;
	auto dir = glm::normalize(pVehicleObj->velocity);
	pVehicleObj->velocity = dir * maxSpeed;
}

