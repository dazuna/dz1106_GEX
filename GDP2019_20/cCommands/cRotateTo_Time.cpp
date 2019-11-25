#include "cRotateTo_Time.hpp"
#include <glm/gtc/quaternion.hpp>
unsigned int cRotateTo_Time::next_uniqueID = 2000;

cRotateTo_Time::cRotateTo_Time(std::string name, std::string type, cGameObject* pGO, glm::vec3 EulerAngles, float timeTR)
{
	// weba // unique ID
	this->m_uniqueID = cRotateTo_Time::next_uniqueID;
	cRotateTo_Time::next_uniqueID++;
	this->m_pTheGO = pGO;
	this->m_Name = name;
	this->m_Type = type;

	this->m_UpdateHasBeeCalled = false;
	this->m_endOrientation = glm::quat(glm::radians(EulerAngles));
	this->m_TimeToRotate = timeTR;
}

void cRotateTo_Time::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO; return;
}

void cRotateTo_Time::Update(double deltaTime)
{
	if ( ! this->m_UpdateHasBeeCalled )
	{
		this->m_startOrientation = this->m_pTheGO->getQOrientation();
		this->m_endOrientation = this->m_startOrientation * this->m_endOrientation;
		// https://stackoverflow.com/questions/22157435/difference-between-the-two-quaternions
		//glm::quat invStart = glm::inverse(this->m_startOrientation);
		//glm::quat qRotationChange = invStart * this->m_endOrientation;
		// How "fast" do we rotation this? 
		//this->m_rotationalSpeed = qRoationChange / this->m_TimeToRotate;
		this->m_UpdateHasBeeCalled = true;
	}

	// get a number between 0.0 and 1.0f
	float AmountOfRotationCompleted = (float)this->m_TimeElapsed / this->m_TimeToRotate; 

	glm::quat qCurrentRotation 
		= glm::slerp( this->m_startOrientation, this->m_endOrientation, 
					  AmountOfRotationCompleted );

	this->m_pTheGO->setOrientation(qCurrentRotation);
	this->m_TimeElapsed += deltaTime;

	return;
}

bool cRotateTo_Time::IsDone(void)
{
	if (this->m_TimeElapsed >= this->m_TimeToRotate)
	{
		return true;
	}
	return false;
}
