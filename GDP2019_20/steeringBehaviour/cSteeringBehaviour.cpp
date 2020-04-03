#include "cSteeringBehaviour.hpp"

// ################# UTIL FUNCTIONS ###########################

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

// ################# LOCAL FUNCTIONS ###########################

glm::vec3 cSteeringBehaviour::seekBhvr(glm::vec3 vehiclePosition,
	glm::vec3 vehicleVelocity, glm::vec3 target,
	float deltaTime, float maxVelocity)
{
	glm::vec3 resultVelocity = vehicleVelocity;

	glm::vec3 desiredVelocity = target - vehiclePosition;
	float dist = glm::length(desiredVelocity);
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= maxVelocity;
	glm::vec3 steeringForce = desiredVelocity - vehicleVelocity;
	resultVelocity += steeringForce * deltaTime;
	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

glm::vec3 cSteeringBehaviour::fleeBhvr(glm::vec3 vehiclePosition,
	glm::vec3 vehicleVelocity, glm::vec3 target,
	float deltaTime, float maxVelocity)
{
	glm::vec3 resultVelocity = vehicleVelocity;

	glm::vec3 desiredVelocity = vehiclePosition - target;
	float dist = glm::length(desiredVelocity);
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= maxVelocity;
	glm::vec3 steeringForce = desiredVelocity - vehicleVelocity;
	resultVelocity += steeringForce * deltaTime;
	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

glm::vec3 cSteeringBehaviour::arriveBhvr(glm::vec3 vehiclePosition,
	glm::vec3 vehicleVelocity, glm::vec3 target,
	float deltaTime, float maxVelocity)
{
	float slowingRadius = 5.0f;
	glm::vec3 resultVelocity = vehicleVelocity;

	glm::vec3 desiredVelocity = target - vehiclePosition;
	float dist = glm::length(desiredVelocity);
	desiredVelocity = glm::normalize(desiredVelocity);
	
	if (dist < slowingRadius)
	{
		desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desiredVelocity *= maxVelocity;
	}

	glm::vec3 steeringForce = desiredVelocity - vehicleVelocity;
	resultVelocity += steeringForce * deltaTime;
	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

glm::vec3 cSteeringBehaviour::wanderBhvr(glm::vec3 vehiclePosition,
	glm::vec3 vehicleVelocity,
	float deltaTime, float maxVelocity)
{
	float circleRadius = 3.f;
	glm::vec3 resultVelocity = glm::vec3(0);

	if (glm::length(vehicleVelocity) == 0)
	{
		vehicleVelocity.x = randInRange(1.f, 2.f);
		vehicleVelocity.y = 0.f; 
		vehicleVelocity.z = randInRange(1.f, 2.f);
	}

	glm::vec3 circlePoint = (glm::normalize(vehicleVelocity) * 5.f) + vehiclePosition;
	float angle = randInRange(0.f, glm::pi<float>() * 2);
	float x = glm::sin(angle) * circleRadius;
	float z = glm::cos(angle) * circleRadius;

	glm::vec3 targetPosition = glm::vec3(circlePoint.x + x, vehiclePosition.y, circlePoint.z + z );
	glm::vec3 desiredVelocity = glm::normalize(targetPosition - vehiclePosition);
	desiredVelocity *= maxVelocity;

	glm::vec3 steer = desiredVelocity - vehicleVelocity;
	resultVelocity = vehicleVelocity + (steer * deltaTime);
	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

glm::vec3 cSteeringBehaviour::pursueBhvr(
	glm::vec3 vehiclePosition, glm::vec3 vehicleVelocity, 
	glm::vec3 target, glm::vec3 targetVelocity,
	float deltaTime, float maxVelocity)
{
	glm::vec3 resultVelocity = glm::vec3(0);
	glm::vec3 distance = target - vehiclePosition;
	int T = (int)glm::length(distance) / (int)maxVelocity;

	glm::vec3 futurePosition = target + targetVelocity * (float)T;

	glm::vec3 desiredVelocity = futurePosition - vehiclePosition;

	desiredVelocity = glm::normalize(desiredVelocity);

	desiredVelocity *= maxVelocity;

	glm::vec3 steer = desiredVelocity - vehicleVelocity;

	resultVelocity = vehicleVelocity + (steer * deltaTime);

	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

glm::vec3 cSteeringBehaviour::evadeBhvr(
	glm::vec3 vehiclePosition, glm::vec3 vehicleVelocity,
	glm::vec3 target, glm::vec3 targetVelocity,
	float deltaTime, float maxVelocity)
{
	glm::vec3 resultVelocity = glm::vec3(0);
	glm::vec3 distance = target - vehiclePosition;
	int T = (int)glm::length(distance) / (int)maxVelocity;

	glm::vec3 futurePosition = target + targetVelocity * (float)T;

	glm::vec3 desiredVelocity = vehiclePosition - futurePosition;

	desiredVelocity = glm::normalize(desiredVelocity);

	desiredVelocity *= maxVelocity;

	glm::vec3 steer = desiredVelocity - vehicleVelocity;

	resultVelocity = vehicleVelocity + (steer * deltaTime);

	if (glm::length(resultVelocity) > maxVelocity)
	{
		resultVelocity = glm::normalize(resultVelocity) * maxVelocity;
	}
	return resultVelocity;
}

// ################# CLASS FUNCTIONS ###########################
cSteeringBehaviour::cSteeringBehaviour(/* args */) {}
cSteeringBehaviour::~cSteeringBehaviour(/* args */) {}

glm::vec3 cSteeringBehaviour::Update(glm::vec3 vehiclePosition, 
	glm::vec3 vehicleVelocity, glm::vec3 target, glm::vec3 targetVelocity,
	float deltaTime, steeringType sType)
{
	glm::vec3 resultVelocity = glm::vec3(0);

	switch (sType)
	{
	case steeringType::seek:
		resultVelocity = seekBhvr(vehiclePosition, 
			vehicleVelocity, target, 
			deltaTime, 40.f);
		break;
	case steeringType::flee:
		resultVelocity = fleeBhvr(vehiclePosition, 
			vehicleVelocity, target, deltaTime);
		break;
	case steeringType::arrive:
		resultVelocity = arriveBhvr(vehiclePosition, 
			vehicleVelocity, target, deltaTime);
		break;
	case steeringType::wander:
		resultVelocity = wanderBhvr(vehiclePosition,
			vehicleVelocity, deltaTime);
		break;
	case steeringType::pursue:
		resultVelocity = pursueBhvr(vehiclePosition, 
			vehicleVelocity, target, 
			targetVelocity, deltaTime);
		break;
	case steeringType::evade:
		resultVelocity = evadeBhvr(vehiclePosition, 
			vehicleVelocity, target, 
			targetVelocity, deltaTime);
		break;
	default:
		break;
	}

	return resultVelocity;
}



