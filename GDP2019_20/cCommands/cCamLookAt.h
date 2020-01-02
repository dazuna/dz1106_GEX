#pragma once
#include "iCommand.h"

class cCamLookAt : public iCommand
{
public:
	cCamLookAt(std::string name, std::string type, glm::vec3* posXYZ, float TimeToLook);
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
	cGameObject* m_pTheGO;
	glm::vec3* m_posXYZ;

	float m_TimeToLook;

	float m_timer;
};