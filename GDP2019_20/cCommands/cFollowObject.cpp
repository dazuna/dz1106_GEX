#include "cFollowObject.hpp"
unsigned int cFollowObject::next_uniqueID = 4000;

cFollowObject::cFollowObject(std::string name, std::string type,
	glm::vec3* followerPos, glm::vec3* targetPos,
	float TimeToFollow, glm::vec3 offsetTarget,
	float maxDistance_MaxVelocity, float minDistance_ZeroVelocity,
	float maxSpeed)
{
	this->fPosition = followerPos;
	this->tPosition = targetPos;
	this->m_TimeToFollow = TimeToFollow;
	this->m_timer = 0.0f;
	this->offsetFromTargetXYZ = offsetTarget;
	this->maxDistance_MaxVelocity = maxDistance_MaxVelocity;
	this->minDistance_ZeroVelocity = minDistance_ZeroVelocity;
	this->maxSpeed = maxSpeed;

	// weba // unique ID
	this->m_uniqueID = cFollowObject::next_uniqueID;
	cFollowObject::next_uniqueID++;
	this->m_Name = name;
	this->m_Type = type;

	return;
}

void cFollowObject::Update(double deltaTime)
{
	glm::vec3 idealPositionXYZ = *(this->tPosition) + this->offsetFromTargetXYZ;
	glm::vec3 vecDirection = glm::normalize(idealPositionXYZ - *(this->fPosition));
	float distanceToTarget = glm::distance( *this->fPosition, idealPositionXYZ);
	glm::vec3 velocityXYZ = vecDirection * this->maxSpeed;
	if (distanceToTarget > this->maxDistance_MaxVelocity)
	{
		// Full speed in the direction I need to go
		velocityXYZ = vecDirection * this->maxSpeed;
	}
	else if (distanceToTarget < this->minDistance_ZeroVelocity)
	{
		// Velocity is zero 
		velocityXYZ = glm::vec3(0.0f,0.0f,0.0f);
	}
	else
	{
		float distanceSpeedRatio = 1.0f; 
		float differenceBetweenCircles = this->maxDistance_MaxVelocity - this->minDistance_ZeroVelocity;
		float distanceToInnerCircle = distanceToTarget - this->minDistance_ZeroVelocity;
		distanceSpeedRatio = distanceToInnerCircle / differenceBetweenCircles;
		float smoothDistanceSpeedRatio = glm::smoothstep( 0.0f, 1.0f, distanceSpeedRatio);
		velocityXYZ *= smoothDistanceSpeedRatio;
	}
	glm::vec3 movementThisStep = velocityXYZ * (float)deltaTime;
	*(this->fPosition) += movementThisStep;

	this->m_timer += (float)deltaTime;
	return;
}

bool cFollowObject::IsDone(void)
{
	if (this->m_timer >= this->m_TimeToFollow) { return true; }
	return false;
}

void cFollowObject::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO; return;
}
