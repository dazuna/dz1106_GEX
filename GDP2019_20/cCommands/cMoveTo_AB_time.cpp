#include "cMoveTo_AB_Time.h"
unsigned int cMoveTo_AB_Time::next_uniqueID = 1000;

cMoveTo_AB_Time::cMoveTo_AB_Time(std::string name, std::string type, glm::vec3* posXYZ, glm::vec3 startPosition, glm::vec3 endPosition, float TimeToMove, float easeIn, float easeOut)
{
	this->m_startPosition = startPosition;
	this->m_endPosition = endPosition;
	this->m_TimeToMove = TimeToMove;
	this->m_timer = 0.0f;
	this->m_easeIn = easeIn;
	this->m_easeOut = easeOut;
	this->m_easeOutBegin = TimeToMove - easeOut;
	this->m_pTheGO = NULL;
	this->m_posXYZ = posXYZ;
	this->m_UpdateHasBeeCalled = false;
	this->m_Name = name;
	this->m_Type = type;

	float realTimeToMove = TimeToMove - easeIn - easeOut;
	if (realTimeToMove < 1.0f)
		realTimeToMove = 1.0f;

	// weba
	// unique ID
	this->m_uniqueID = cMoveTo_AB_Time::next_uniqueID;
	cMoveTo_AB_Time::next_uniqueID++;

	// Calculate the velocity...
	glm::vec3 moveVector = this->m_endPosition - this->m_startPosition;
	this->m_totalDistance = glm::length(moveVector);
	float speed = this->m_totalDistance / (realTimeToMove + (this->m_easeIn / 2) + (this->m_easeOut / 2));
	glm::vec3 direction = glm::normalize(moveVector);
	this->m_topVelocity = direction * speed;

	return;
}

void cMoveTo_AB_Time::Update(double deltaTime)
{
	if ( ! this->m_UpdateHasBeeCalled )
	{
		*(this->m_posXYZ) = this->m_startPosition;
		this->m_UpdateHasBeeCalled = true;
	}
	glm::vec3 deltaVelocity = this->m_topVelocity;
	float easePercent;
	if (this->m_timer < this->m_easeIn)
	{
		easePercent = glm::smoothstep(0.0f, this->m_easeIn, this->m_timer);
		deltaVelocity *= easePercent;
	}
	if (this->m_timer > this->m_easeOutBegin)
	{
		easePercent = glm::smoothstep(this->m_TimeToMove, this->m_easeOutBegin, this->m_timer);
		//std::cout << "ease out!! " << easePercent << " ";
		deltaVelocity *= easePercent;
	}
	//std::cout << "timer: " << this->m_timer << ",  deltaVelocity: " << glm::to_string(deltaVelocity) << std::endl;
	glm::vec3 deltaStep = deltaVelocity * (float)deltaTime;
	*(this->m_posXYZ) += deltaStep;
	this->m_timer += (float)deltaTime;
	return;
}

bool cMoveTo_AB_Time::IsDone(void)
{
	//float distance = glm::distance(*(this->m_posXYZ), this->m_startPosition );
	//if (distance >= this->m_totalDistance) { return true; }
	if (this->m_timer >= this->m_TimeToMove) { return true; }
	return false;
}

void cMoveTo_AB_Time::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO;
	return;
}