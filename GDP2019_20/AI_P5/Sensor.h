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
	Sensor() {}
	void updatePosition();
	float getCollisionFuzzyValue();
	void drawSensor(glm::vec3 pos);
private:
	bool checkCollision(cGameObject* A, cGameObject* B, glm::vec3& collisionPos);
};