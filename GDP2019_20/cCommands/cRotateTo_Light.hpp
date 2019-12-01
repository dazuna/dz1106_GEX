#pragma once
#include "iCommand.h"
#include "../cLight.h"

class cRotateTo_Light : public iCommand
{
public:
	cRotateTo_Light(std::string name, std::string type, cLight* pGO,
		glm::vec3 EulerAngles, float timeTR);
	cRotateTo_Light(std::string name, std::string type, cLight* pGO, 
		glm::vec3 EulerAngles, float timeTR, int easeIn, int easeOut);
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
	bool m_UpdateHasBeenCalled;
	std::string m_Type;
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
	std::string m_Name;

	cGameObject* m_pTheGO;
	cLight* m_pTheLight;

	glm::quat m_startOrientation;		// Starting location
	glm::quat m_endOrientation;			// Ending location
	glm::quat m_rotationalSpeed;		// Like velocity
	float m_TimeToRotate;

	float m_easeIn, m_easeOut,
		m_easeOutBegin, m_deltaRotation;
	glm::quat m_firstEaseOutRotation;
	double m_timer;
};