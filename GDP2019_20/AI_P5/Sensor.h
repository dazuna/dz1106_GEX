#pragma once
#include <vector>
#include "../cGameObject.h"

struct Sensor
{
	std::vector<cGameObject*> sensorSpheres;
	cGameObject* vehicle = nullptr;
	float maxDistance = 0.0f, offsetAngle = 0.0f, distanceBetween = 0.0f;
	int nObjects = 3;

	Sensor(cGameObject* vehicle, float distanceBetween, float angle);
	void updatePosition();
	float getCollisionFuzzyValue(const std::vector<cGameObject*>& colliders);
	void drawSensor(glm::vec3 pos);
};