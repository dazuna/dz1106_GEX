#pragma once
#include "iCommand.h"

class cTrigger : public iCommand
{
public:
	cTrigger(std::string name, std::string type, 
	glm::vec3* itemToMonitor, glm::vec3 startPos, iCommand* theCommand,
	float radius, float TimeToExist = 10.0f);

	virtual void SetGameObject(cGameObject* pGO);
	virtual void Update(double deltaTime);
	virtual bool IsDone(void);

	virtual void setName(std::string name) { this->m_Name = name; }
	virtual std::string getName(void) { return this->m_Name; }
    virtual unsigned int getUniqueID(void) { return this->m_uniqueID; }
    virtual void setType(std::string type) { this->m_Type = type; }
	virtual std::string getType(void) { return this->m_Type; }

private:
	std::string m_Name;
	std::string m_Type;
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

	float m_TimeToExist;
	glm::vec3 m_position;
	glm::vec3* m_target;
	float m_radius;
	bool m_isTriggerOn;
	float m_timer;
	iCommand* m_theCommand;
};
