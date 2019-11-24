#pragma once
#include "../cGameObject.h"
#include <string>

class iCommand
{
public:
	virtual ~iCommand() {};
	virtual void SetGameObject( cGameObject* pGO ) = 0;
	virtual void Update( double deltaTime ) = 0;
	virtual bool IsDone(void) = 0;

	virtual void setName(std::string name) = 0;
	virtual std::string getName(void) = 0;
    virtual unsigned int getUniqueID(void) = 0;
    virtual void setType(std::string type) = 0;
	virtual std::string getType(void) = 0;
};
