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
	if (this->IsDone()) { return; }

	iCommand* pCurrent = *this->vecCommands.begin();
	if (!pCurrent->IsDone())
	{
		pCurrent->Update(deltaTime);
	}
	else
	{
		delete pCurrent;
		this->vecCommands.erase(this->vecCommands.begin());
	}
	return;
}

bool cCommandGroupSerial::IsDone(void)
{
	if (this->vecCommands.empty()) { return true; }
	return false;
}

void cCommandGroupSerial::AddCommandSerial(iCommand* pCommand)
{
	this->vecCommands.push_back(pCommand);
}

void cCommandGroupSerial::SetGameObject(cGameObject* pGO)
{
	return;
}
