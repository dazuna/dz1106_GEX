#pragma once
#include "iCommand.h"
#include "../cGameObject.h"
#include <string>
#include <vector>

class cCommandGroupParallel : public iCommand
{
public: 
	cCommandGroupParallel(std::string name, std::string type);
	virtual void SetGameObject(cGameObject* pGO);
	virtual void Update(double deltaTime);
	virtual bool IsDone(void);

	virtual void setName(std::string name) { this->m_Name = name; }
	virtual std::string getName(void) { return this->m_Name; }
    virtual unsigned int getUniqueID(void) { return this->m_uniqueID; }
    virtual void setType(std::string type) { this->m_Type = type; }
	virtual std::string getType(void) { return this->m_Type; }

	void AddCommandParallel(iCommand* pCommand);

private:
	std::string m_Name;
	std::string m_Type;
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

	std::vector< iCommand* > vecCommands;
};