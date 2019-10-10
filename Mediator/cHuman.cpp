#include "cHuman.h"

// include the ACTUAL header
#include "cHuman_Imp.h"

cHuman::cHuman()
{
	this->pTheImp = new cHuman_Imp();
}
cHuman::~cHuman()
{
	delete this->pTheImp;
}


sNVPair cHuman::RecieveMessage(sNVPair message)
{
	sNVPair response;
	response.name = "UNKNOWN";

	if (message.name == "You suck as an actor")
	{
		this->pTheImp->shoesize -= 0.5f;
		response.name = "OK";
		return response;
	}

	return response;
}


void cHuman::RunAway(void)
{
	this->pTheImp->RunAway();
}
