#pragma once
#include "iCommand.h"

class cMoveTo_AB_Time : public iCommand
{
public:
	cMoveTo_AB_Time(std::string name, std::string type, glm::vec3* posXYZ, glm::vec3 startPosition, glm::vec3 endPosition, float TimeToMove, float easeIn, float easeOut);
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
	bool m_UpdateHasBeeCalled;
	cGameObject* m_pTheGO;
	glm::vec3* m_posXYZ;

	glm::vec3 m_startPosition;
	glm::vec3 m_endPosition;
	float m_TimeToMove;

	glm::vec3 m_topVelocity;
	float m_totalDistance;
	float m_timer;
	float m_easeIn;
	float m_easeOut;
	float m_easeOutBegin;
	//float m_easeOutDistBegin;
};