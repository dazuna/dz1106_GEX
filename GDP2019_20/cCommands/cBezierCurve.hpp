#pragma once
#include "iCommand.h"

class cBezierCurve : public iCommand
{
public:
	cBezierCurve(std::string name, std::string type,
		glm::vec3* itemToMove,
		glm::vec3 startPos, glm::vec3 endPos,
		glm::vec3 ctlPos, float timeToMove,
		float easeIn, float easeOut);
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

	float m_timer;
	float m_TimeToCurve;
	float m_easeIn;
	float m_easeOut;
	float m_easeOutBegin;
	float max_Vel;
	float t;

	glm::vec3* m_itemToMove;
	glm::vec3 m_startPos;
	glm::vec3 m_endPos;
	glm::vec3 m_ctlPos;
};
