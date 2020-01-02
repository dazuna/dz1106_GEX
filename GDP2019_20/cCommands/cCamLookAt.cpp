#include "cCamLookAt.h"
#include "../globalStuff.h"
unsigned int cCamLookAt::next_uniqueID = 8000;

cCamLookAt::cCamLookAt(std::string name, std::string type, glm::vec3* posXYZ, float TimeToLook)
{
	this->m_TimeToLook = TimeToLook;
	this->m_timer = 0.0f;
	this->m_pTheGO = NULL;
	this->m_posXYZ = posXYZ;
	this->m_Name = name;
	this->m_Type = type;

	// weba
	// unique ID
	this->m_uniqueID = cCamLookAt::next_uniqueID;
	cCamLookAt::next_uniqueID++;

	return;
}

void cCamLookAt::Update(double deltaTime)
{
	::g_pFlyCamera->cameraLookAt(*this->m_posXYZ);
	this->m_timer += deltaTime;
	return;
}

bool cCamLookAt::IsDone(void)
{
	if (this->m_timer >= this->m_TimeToLook) { return true; }
	return false;
}

void cCamLookAt::SetGameObject(cGameObject* pGO)
{
	this->m_pTheGO = pGO;
	return;
}