#pragma once
#include "iCommand.h"

class cOrientTo_Time : public iCommand
{
public:
	cOrientTo_Time(std::string name, std::string type, cGameObject* pGO,glm::vec3 EulerAngles, float timeTR);
	virtual void SetGameObject(cGameObject* pGO);
	virtual void Update(double deltaTime);
	virtual bool IsDone(void);

	virtual void setName(std::string name) { this->m_Name = name; }
	virtual std::string getName(void) { return this->m_Name; }
	virtual unsigned int getUniqueID(void) { return this->m_uniqueID; }
    virtual void setType(std::string type) { this->m_Type = type; }
	virtual std::string getType(void) { return this->m_Type; }

private:

	// Flag to see if update has been called once
	bool m_UpdateHasBeeCalled;
	std::string m_Type;
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
	std::string m_Name;

	cGameObject* m_pTheGO;

	glm::quat m_startOrientation;		// Starting location
	glm::quat m_endOrientation;			// Ending location
	glm::quat m_rotationalSpeed;		// Like velocity
	float m_TimeToRotate;

	double m_TimeElapsed;
};