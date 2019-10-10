#include "cWorld.h"
#include <iostream>
#include <sstream>


void cWorld::GenerateWorld(int numRobots, int numBatteries, float worldSize)
{
	std::vector<std::string> vecFirstNames;
	vecFirstNames.push_back("Emma");
	vecFirstNames.push_back("Liam");
	vecFirstNames.push_back("Noah");
	vecFirstNames.push_back("Olivia");
	vecFirstNames.push_back("Ava");
	vecFirstNames.push_back("Isabella");
	vecFirstNames.push_back("Sophia");
	vecFirstNames.push_back("Elijah");
	vecFirstNames.push_back("Logan");
	vecFirstNames.push_back("Mia");
	vecFirstNames.push_back("Mason");
	vecFirstNames.push_back("James");
	vecFirstNames.push_back("Aiden");
	vecFirstNames.push_back("Ethan");
	vecFirstNames.push_back("Lucas");
	vecFirstNames.push_back("Jacob");
	vecFirstNames.push_back("Michael");
	vecFirstNames.push_back("Matthew");
	vecFirstNames.push_back("Benjamin");
	vecFirstNames.push_back("Amelia");
	vecFirstNames.push_back("Charlotte");
	vecFirstNames.push_back("Alexander");
	vecFirstNames.push_back("William");
	vecFirstNames.push_back("Daniel");

	std::vector<std::string> vecLastNames;
	vecLastNames.push_back("SMITH");
	vecLastNames.push_back("JOHNSON");
	vecLastNames.push_back("WILLIAMS");
	vecLastNames.push_back("JONES");
	vecLastNames.push_back("BROWN");
	vecLastNames.push_back("DAVIS");
	vecLastNames.push_back("MILLER");
	vecLastNames.push_back("WILSON");
	vecLastNames.push_back("MOORE");
	vecLastNames.push_back("TAYLOR");
	vecLastNames.push_back("ANDERSON");
	vecLastNames.push_back("THOMAS");
	vecLastNames.push_back("JACKSON");
	vecLastNames.push_back("WHITE");
	vecLastNames.push_back("HARRIS");
	vecLastNames.push_back("MARTIN");
	vecLastNames.push_back("THOMPSON");
	vecLastNames.push_back("GARCIA");
	vecLastNames.push_back("MARTINEZ");
	vecLastNames.push_back("ROBINSON");


	for (unsigned int count = 0; count != numRobots; count++)
	{
		cRobot* pRobot = new cRobot();

		pRobot->energy = randInRange(50.0f, 100.0f);

		pRobot->position.x = randInRange(-worldSize, worldSize);
		pRobot->position.y = randInRange(-worldSize, worldSize);

		std::stringstream ssName;
		ssName 
			<< vecFirstNames[rand() % (vecFirstNames.size() - 1)] << " " 
			<< vecFirstNames[rand() % (vecFirstNames.size() - 1)] << " " 
			<< vecFirstNames[rand() % (vecLastNames.size() - 1)];

		pRobot->name = ssName.str();

		// This might make more sense:
		iMessageInterface* pWorldInterface = this;
		pRobot->setWorldPointer(pWorldInterface);


		std::cout << "Made a robot " 
			<< pRobot->getUniqueID() << ":" 
			<< pRobot->name 
			<< " at " 
			<< pRobot->position.x << ", "
			<< pRobot->position.y 
			<< " with " 
			<< pRobot->energy << " energy" << std::endl;

		this->m_vec_pRobots.push_back(pRobot);
	}

	std::cout << std::endl;

	for (unsigned int count = 0; count != numBatteries; count++)
	{
		cBattery* pBattery = new cBattery();
		pBattery->amountPower = randInRange( 500.0f, 10000.0f );
		pBattery->location.x = randInRange(-worldSize, worldSize);
		pBattery->location.y = randInRange(-worldSize, worldSize);

		std::cout << "Made a battery at " 
			<< pBattery->location.x << ", "
			<< pBattery->location.y
			<< " with " 
			<< pBattery->amountPower << " power" << std::endl;

		this->m_vec_pBatteries.push_back(pBattery);
	}

	this->m_vec_pHumans.push_back( new cHuman() );

	std::cout << std::endl;

	return;
}


float cWorld::getWorldSize(void)
{
	return this->m_worldSize;
}

sNVPair cWorld::RecieveMessage(sNVPair message)
{

	sNVPair response;
	response.name = "OK";

	if (message.name == "Take Energy from Largest Battery")
	{
		cBattery* pBattery = this->findLargestBattery();
		sNVPair message;
		message.name = "Take Energy";
		message.fValue = message.fValue;
		pBattery->RecieveMessage(message);

		return response;
	}

	if (message.name == "Make fun of actor")
	{
		sNVPair message;
		message.name = "You suck as an actor";
		this->m_vec_pHumans[0]->RecieveMessage(message);
		response.name = "OK";
		return response;
	}
	// 
	response.name = "UNKNOWN COMMAND";
	return response;
}


cBattery* cWorld::findLargestBattery()
{
	// TODO: Find largest battery

	// HACK: for now, just return the 1st one
	return this->m_vec_pBatteries[0];

	return 0;
}
