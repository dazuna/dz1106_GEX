#pragma once
#include "sNVPair.h"

class iMessageInterface
{
public: 
	virtual ~iMessageInterface() {};

	virtual sNVPair RecieveMessage(sNVPair message) = 0;

	// Database:
//	recordSet DoTheSQL( string theSQL);

//	TakeThisURLStuff( string thePOSTGET );
};


