#include "cTrigger.hpp"
unsigned int cTrigger::next_uniqueID = 5000;

cTrigger::cTrigger(std::string name, std::string type,
	glm::vec3* itemToMonitor, glm::vec3 startPos, iCommand* theCommand,
	float radius, float TimeToExist)
{
	this->m_TimeToExist = TimeToExist;
	this->m_position = startPos;
	this->m_target = itemToMonitor;
	this->m_radius = radius;
	this->m_isTriggerOn = false;
	this->m_timer = 0.0f;
	this->m_theCommand = theCommand;
	
	// weba // unique ID
	this->m_uniqueID = cTrigger::next_uniqueID;
	cTrigger::next_uniqueID++;
	this->m_Name = name;
	this->m_Type = type;

	return;
}

void cTrigger::Update(double deltaTime)
{
	float distance = glm::distance(this->m_position,*(this->m_target));
	//std::cout << "dist: " << distance << std::endl;
	if(distance <= this->m_radius)
	{
		this->m_isTriggerOn = true;
	}
	if(this->m_isTriggerOn)
	{
		this->m_theCommand->Update(deltaTime);
		if(this->m_theCommand->IsDone())
		{
			std::cout << "trigger is done!!" << std::endl;
			this->m_isTriggerOn = false;
		}
	}
	this->m_timer += deltaTime;
	return;
}

bool cTrigger::IsDone(void)
{
	if(!this->m_isTriggerOn)
	{
		// std::cout << "trigger is off" 
		// << " timer: " << this->m_timer
		// << " timeToLive: " << this->m_TimeToExist
		// << std::endl;
		if (this->m_timer >= this->m_TimeToExist) 
		{
			std::cout << "trigger finished" << std::endl;
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}	
}

void cTrigger::SetGameObject(cGameObject* pGO) { return; }
