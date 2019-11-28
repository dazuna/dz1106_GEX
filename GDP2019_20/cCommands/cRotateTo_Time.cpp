#include "cRotateTo_Time.hpp"
#include <glm/gtc/quaternion.hpp>
unsigned int cRotateTo_Time::next_uniqueID = 2000;

cRotateTo_Time::cRotateTo_Time(std::string name, std::string type, cGameObject* pGO,
	glm::vec3 EulerAngles, float timeTR)
{
	// weba // unique ID
	this->m_uniqueID = cRotateTo_Time::next_uniqueID;
	cRotateTo_Time::next_uniqueID++;
	this->m_pTheGO = pGO;
	this->m_Name = name;
	this->m_Type = type;

	this->m_UpdateHasBeenCalled = false;
	this->m_endOrientation = glm::quat(glm::radians(EulerAngles));
	this->m_TimeToRotate = timeTR;
	this->m_easeIn = 0.0f;
	this->m_easeOut = 0.0f;
	this->m_easeOutBegin = this->m_TimeToRotate - this->m_easeOut;
}

cRotateTo_Time::cRotateTo_Time(std::string name, std::string type, cGameObject* pGO,
	glm::vec3 EulerAngles, float timeTR, int easeIn, int easeOut)
{
	// weba // unique ID
	this->m_uniqueID = cRotateTo_Time::next_uniqueID;
	cRotateTo_Time::next_uniqueID++;
	this->m_pTheGO = pGO;
	this->m_Name = name;
	this->m_Type = type;

	this->m_UpdateHasBeenCalled = false;
	this->m_endOrientation = glm::quat(glm::radians(EulerAngles));
	this->m_TimeToRotate = timeTR;
	this->m_easeIn = (float)easeIn;
	this->m_easeOut = (float)easeOut;
	this->m_easeOutBegin = this->m_TimeToRotate - this->m_easeOut;
}

void cRotateTo_Time::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO; return;
}

void cRotateTo_Time::Update(double deltaTime)
{
	if (!this->m_UpdateHasBeenCalled)
	{
		this->m_startOrientation = this->m_pTheGO->getQOrientation();
		this->m_endOrientation = this->m_startOrientation * this->m_endOrientation;
		// https://stackoverflow.com/questions/22157435/difference-between-the-two-quaternions
		//glm::quat invStart = glm::inverse(this->m_startOrientation);
		//glm::quat qRotationChange = invStart * this->m_endOrientation;
		// How "fast" do we rotation this? 
		//this->m_rotationalSpeed = qRoationChange / this->m_TimeToRotate;
		this->m_UpdateHasBeenCalled = true;
		
	}
	float realTimeToMove = m_TimeToRotate - m_easeIn - m_easeOut;
	float max_RotVel = 1 / (realTimeToMove + (this->m_easeIn / 2) + (this->m_easeOut / 2));

	float easePercent = 1.0f;
	if (this->m_timer < this->m_easeIn)
	{
		easePercent = glm::smoothstep(0.0f, this->m_easeIn, (float)this->m_timer);
	}
	if (this->m_timer > this->m_easeOutBegin)
	{
		easePercent = glm::smoothstep(this->m_TimeToRotate, this->m_easeOutBegin, (float)this->m_timer);
	}

	max_RotVel *= easePercent;
	this->m_deltaRotation += max_RotVel * deltaTime;

	glm::quat qCurrentRotation
		= glm::slerp(this->m_startOrientation, this->m_endOrientation, m_deltaRotation);

	this->m_pTheGO->setOrientation(qCurrentRotation);
	this->m_timer += deltaTime;

	return;
}

bool cRotateTo_Time::IsDone(void)
{
	if (this->m_timer >= this->m_TimeToRotate)
	{
		return true;
	}
	return false;
}
