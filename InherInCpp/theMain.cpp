//#include "cShip.h"
//#include "cFastShip.h"
//#include "cBigFastShip.h"
//#include "cSmallShip.h"
//#include "cLASER.h"
//#include "cBigFreakingLASER.h"
#include "cShipFactory.h"
#include "cWorld.h"
#include "cWorld2.h"

#include "cRobot.h"
#include "cRobotWorld.h"

#include <iostream>
#include <vector>
#include <fstream>

// Another solution to the single threading issue


int main()
{
	cRobotWorld* pTheRobotWorld= new cRobotWorld();

	cRobot* pR1 = new cRobot();
	cRobot* pR2 = new cRobot();

	pR1->SetWorldPointer( (iMediatorInterface*) pTheRobotWorld);
	pR2->SetWorldPointer( (iMediatorInterface*) pTheRobotWorld);

	pTheRobotWorld->AddRobotToWorld(pR1);
	pTheRobotWorld->AddRobotToWorld(pR2);



	std::vector<iShip*> myShips;

	// Load the info from the file
	std::ifstream shipFile("MyShips.txt");
	if (!shipFile.is_open())
	{
		std::cout << "Didn't find file" << std::endl;
		return -1;
	}


	cShipFactory* pFactory = new cShipFactory();

//	cWorld::x = 1;

	//cWorld* pTheWorld = new cWorld();
	cWorld* pTheWorld = cWorld::CreateAWorld_NotThreadSafe();
	cWorld* pTheWorld2 = cWorld::CreateAWorld_NotThreadSafe();
	cWorld* pTheWorld3 = cWorld::CreateAWorld_NotThreadSafe();

	cWorld* pTheWorld4 = cWorld::CreateAWorld_ThreadSafe();
	cWorld* pTheWorld5 = cWorld::CreateAWorld_ThreadSafe();
	cWorld* pTheWorld6 = cWorld::CreateAWorld_ThreadSafe();


	cWorld2* pTheWorld7 = cWorld2::GetWorld_ThreadSafe();
	cWorld2* pTheWorld8 = cWorld2::GetWorld_ThreadSafe();
	cWorld2* pTheWorld9 = cWorld2::GetWorld_ThreadSafe();

//	cWorld* pTheWorld2 = new cWorld();

	std::string tempShipType;
	while (shipFile >> tempShipType)
	{
		if (tempShipType == "EOF") { break; }

		std::cout << "About to creat a " 
			<< tempShipType << " ship..." << std::endl;

		iShip* pTheShip = pFactory->CreateShip(tempShipType);

		//iShip* pTheShip = OLD_SCHOOL_CreateShip(tempShipType);

//		pTheWorld->AddShipToWorld(pTheShip);

		myShips.push_back( pTheShip);

		std::cout << std::endl;
	}

	iShip* pPoorShipThatsGonnaGetShot = myShips[5];

	myShips[0]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[1]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[2]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[3]->Shoot(pPoorShipThatsGonnaGetShot);

	pFactory->UpgradeShip(myShips[0], "Ship upgrade to fancy LASER");

	myShips[0]->Shoot(pPoorShipThatsGonnaGetShot);

//	iShip* pShipA = new cShip();
//	myShips.push_back( pShipA );
//
//	iShip* pShipB = new cFastShip();
//	myShips.push_back( pShipB );
//
//	iShip* pShipC = new cBigFastShip();
//	myShips.push_back( pShipC );
//
//	iShip* pShipD = new cSmallShip();
//	myShips.push_back( pShipD );

	//std::cout << std::endl;
	//pShipA->Shoot(pShipB);

	//cShip* pSS = new cShip();
	//pSS->Fly();
	//pSS->myLASER.Fire();

	//cBigFastShip* pBFS = new cBigFastShip();
	//pBFS->myLASER.Fire();
	//pBFS->myBFL.Fire();

	//myShips[0]->Fly();
	//myShips[1]->Fly();
	//myShips[2]->Fly();
	//myShips[3]->Fly();



	//cLASER* pL = new cLASER();
	//cBigFreakingLASER* pBFL = new cBigFreakingLASER();


}


//cShip ships[2];			// STACK
