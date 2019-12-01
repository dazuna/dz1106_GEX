#include "cCommandGroupParallel.h"
unsigned int cCommandGroupParallel::next_uniqueID = 1000;

cCommandGroupParallel::cCommandGroupParallel(std::string name, std::string type)
{
	this->m_Name = name;
	this->m_Type = type;
	// weba // unique ID
	this->m_uniqueID = cCommandGroupParallel::next_uniqueID;
	cCommandGroupParallel::next_uniqueID++;

	return;
}

void cCommandGroupParallel::Update(double deltaTime)
{
	if (this->IsDone()) { return; }
	std::vector< iCommand* >::iterator itCmds;
	for (itCmds = this->vecCommands.begin(); itCmds != this->vecCommands.end(); itCmds++)
	{
		iCommand* pCmd = *itCmds;
		if (pCmd->IsDone())
		{
			continue;
		}
		else
		{
			pCmd->Update(deltaTime);
		}
	}
	return;
}

bool cCommandGroupParallel::IsDone(void)
{
	if (this->vecCommands.empty()) { return true; }

	bool allDone = true;
	std::vector< iCommand* >::iterator itCmds;
	for (itCmds = this->vecCommands.begin(); itCmds != this->vecCommands.end(); itCmds++)
	{
		iCommand* pCmd = *itCmds;
		allDone &= pCmd->IsDone();
	}
	return allDone;
}

void cCommandGroupParallel::AddCommandParallel(iCommand* pCommand)
{
	this->vecCommands.push_back(pCommand);
}

void cCommandGroupParallel::SetGameObject(cGameObject* pGO)
{
	return;
}
