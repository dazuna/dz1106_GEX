#include "cBezierCurve.hpp"
unsigned int cBezierCurve::next_uniqueID = 4000;

cBezierCurve::cBezierCurve(std::string name, std::string type,
	glm::vec3* itemToMove,
	glm::vec3 startPos, glm::vec3 endPos,
	glm::vec3 ctlPos, float timeToMove)
{
	this->m_itemToMove = itemToMove;
	this->m_startPos = startPos;
	this->m_endPos = endPos;
	this->m_ctlPos = ctlPos;
	this->m_TimeToCurve = timeToMove;
	this->m_timer = 0.0f;

	// weba // unique ID
	this->m_uniqueID = cBezierCurve::next_uniqueID;
	cBezierCurve::next_uniqueID++;
	this->m_Name = name;
	this->m_Type = type;

	return;
}

void cBezierCurve::Update(double deltaTime)
{
	float t = this->m_timer / this->m_TimeToCurve;
	// Beizer curve equation MAGIC
	glm::vec3 currentPos = (1 - t) * (1 - t) * this->m_startPos + 2 * (1 - t) * t * this->m_ctlPos + t * t * this->m_endPos;

	*(this->m_itemToMove) = currentPos;
	this->m_timer += deltaTime;
	return;
}

bool cBezierCurve::IsDone(void)
{
	if (this->m_timer >= this->m_TimeToCurve) { return true; }
	return false;
}

void cBezierCurve::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO; return;
}