#include "Sensor.h"
#include <glm/gtx/rotate_vector.hpp>
#include "../util/tools.h"
#include "../SceneManager/cSceneManager.h"

Sensor::Sensor(cGameObject* vehicle, float distanceBetween, float angle) :
	vehicle(vehicle),
	offsetAngle(angle),
	distanceBetween(distanceBetween)
{

	// Get the sensorShpere template
	if (!tools::pFindObjectByFriendlyNameMap("sphereSensor"))
	{
		std::cout << "No sphereSensor template!!" << std::endl;
		return;
	}
	auto sensorTemplate = ::g_map_GameObjects["sphereSensor"];
	
	maxDistance = distanceBetween * nObjects + sensorTemplate->SPHERE_radius;
	
	for (int i = 0; i < nObjects; i++)
	{
		auto newDetector = new cGameObject(sensorTemplate);
		newDetector->isVisible = true;
		sensorSpheres.push_back(newDetector);
	}
	updatePosition();
}

void Sensor::updatePosition()
{
	// Figure out the actual direction in the world
	auto vehicleAt = vehicle->getCurrentAT();
	auto sensorDir = glm::rotate(vehicleAt, glm::radians(offsetAngle), vehicle->getCurrentUP());

	float currentDistance = distanceBetween;
	for (auto sphere : sensorSpheres)
	{
		sphere->positionXYZ = vehicle->positionXYZ + sensorDir * currentDistance;
		currentDistance += distanceBetween;
	}
}

float Sensor::getCollisionFuzzyValue(const std::vector<cGameObject*>& colliders)
{
	auto theSceneManager = cSceneManager::getTheSceneManager();
	glm::vec3 collisionPos;
	for (const auto& collider : theSceneManager->scenesVector[0]->sceneGameObjects)
	{
		/*
		 * We check from the closest point to the farthest
		 */
		for (auto sphere : sensorSpheres)
		{
			/*
			 * We return the first collision detected
			 */
			if (checkCollision(sphere, collider.second, collisionPos))
			{
				drawSensor(collisionPos);
				auto collisionVec = collisionPos - vehicle->positionXYZ;
				auto collisionDistance = glm::length(collisionVec);
				auto fuzzyValue = collisionDistance / maxDistance;
				return fuzzyValue;
			}
		}
	}
	drawSensor(sensorSpheres[sensorSpheres.size() - 1]->positionXYZ);
	return 1.0f;
}

void Sensor::drawSensor(glm::vec3 pos)
{
	auto sensorTemplate = ::g_map_GameObjects["sphereSensor"];
	sensorTemplate->isVisible = true;
	sensorTemplate->positionXYZ = pos;
	tools::DrawObject(glm::mat4(), sensorTemplate, 0, nullptr);
	::pDebugRenderer->addLine(vehicle->positionXYZ, pos, { 1,1,1 });
	sensorTemplate->isVisible = false;
}

bool Sensor::checkCollision(cGameObject* pA, cGameObject* pB, glm::vec3& collisionPos)
{
	if (pA->physicsShapeType != SPHERE || pB->physicsShapeType != SPHERE)
	{
		// We only support spheres so far
		return false;
	}
	
	glm::vec3 ABvec = pB->positionXYZ - pA->positionXYZ;
	glm::vec3 BAvec = pA->positionXYZ - pB->positionXYZ;
	if (glm::length(ABvec) <= pA->SPHERE_radius + pB->SPHERE_radius)
	{
		// TODO: get actual collision intersection point between the spheres
		collisionPos = pA->positionXYZ;
		return true;
	}
	return false;
}
