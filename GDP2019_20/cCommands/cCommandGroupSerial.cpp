#include "cCommandGroupSerial.h"
unsigned int cCommandGroupSerial::next_uniqueID = 1000;

cCommandGroupSerial::cCommandGroupSerial(std::string name, std::string type)
{
	this->m_Name = name;
	this->m_Type = type;
	// weba // unique ID
	this->m_uniqueID = cCommandGroupSerial::next_uniqueID;
	cCommandGroupSerial::next_uniqueID++;

	return;
}

void cCommandGroupSerial::Update(double deltaTime)
{
	return;
}

bool cCommandGroupSerial::IsDone(void)
{
	return false;
}

void cCommandGroupSerial::SetGameObject(cGameObject* pGO)
{
	return;
}