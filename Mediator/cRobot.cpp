#include "cRobot.h"
//#include "cBattery.h"
//#include "cHuman.h"
#include "cNVPaid.h"

void cRobot::Update(void)
{
	// 1. Take some energy away 
	// 2. If I'm under some value, look for (25%)
	//    the biggest battery
	// 3. Move toward it
	// 4. Get the energy from that battery
	// (if more than one robot is at the battery, what happens?)

//	cBattery* pBattery = this->m_pTheWorld->findLargestBattery();
//
	float energyToTake = (100.0f - this->energy);
	this->energy += energyToTake;
////	pBattery->amountPower -= energyToTake;
//
//	sNVPair message;
//	message.name = "Take Energy";
//	message.fValue = energyToTake;
//	pBattery->RecieveMessage(message);

	sNVPair message;
	message.name = "Take Energy from Largest Battery";
	message.fValue = energyToTake;
	this->m_pTheWorld->RecieveMessage(message);

	sNVPair message2;
	message2.name = "Make fun of actor";
	this->m_pTheWorld->RecieveMessage(message2);


	// Send a message to the battery, telling it 
	// to take X amount of energy away..


	return;
}

void cRobot::PrintState(void) 
{


	return;
}


unsigned int cRobot::getUniqueID(void)
{
	return this->m_uniqueID;
}


void cRobot::setWorldPointer(iMessageInterface* pTheWorld)
{
	this->m_pTheWorld = pTheWorld;
	return;
}
