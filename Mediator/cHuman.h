#pragma once
#include <string>
#include "iMessageInterface.h"

// Forward declaration...
class cHuman_Imp;

class cHuman : public iMessageInterface
{
public:
	cHuman();
	~cHuman();
	virtual sNVPair RecieveMessage(sNVPair message);

	void RunAway(void);


private:
	cHuman_Imp* pTheImp;		// MUST be a pointer

};