#include "cBattery.h"
#include <iostream>

sNVPair cBattery::RecieveMessage(sNVPair message)
{
	sNVPair response;
	response.name = "UNKNOWN COMMAND";

	if (message.name == "Take Energy")
	{
		this->amountPower -= message.fValue;
		response.name = "OK";
		std::cout << "Battery: " << this->amountPower << std::endl;
		return response;
	}

	return response;
}
