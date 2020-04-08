#include "Sensor.h"
#include <glm/gtx/rotate_vector.hpp>
#include "../util/tools.h"

Sensor::Sensor(cGameObject* vehicle, float distanceBetween, float angle) :
	vehicle(vehicle),
	offsetAngle(angle),
	distanceBetween(distanceBetween)
{
	maxDistance = distanceBetween * nObjects;

	// Get the sensorShpere template
	if (!tools::pFindObjectByFriendlyNameMap("sphereSensor"))
	{
		std::cout << "No sphereSensor template!!" << std::endl;
		return;
	}
	auto sensorTemplate = ::g_map_GameObjects["shpereSensor"];
	
	for (int i = 0; i < nObjects; i++)
	{
		auto newDetector = new cGameObject(sensorTemplate);
		sensorSpheres.push_back(newDetector);
	}
	updatePosition();
}

void Sensor::updatePosition()
{
	// Figure out the actual direction in the world
	auto vehicleAt = vehicle->getCurrentAT();
	auto sensorDir = glm::rotate(vehicleAt, offsetAngle, { 0, 1, 0 });

	float currentDistance = distanceBetween;
	for (auto sphere : sensorSpheres)
	{
		sphere->positionXYZ = vehicle->positionXYZ + sensorDir * currentDistance;
		currentDistance += distanceBetween;
	}
}

float Sensor::getCollisionFuzzyValue(const std::vector<cGameObject*>& colliders)
{
	return 0.0f;
}

void Sensor::drawSensor(glm::vec3 pos)
{}
