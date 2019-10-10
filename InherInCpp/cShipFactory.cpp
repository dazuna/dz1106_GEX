#include "cShipFactory.h"
// 
// In THIS file (cpp file) I include the specific 
// types of ships. 
// AGAIN, DON'T include these in the header file
#include "cShip.h"
#include "cFastShip.h"
#include "cBigFastShip.h"
#include "cSmallShip.h"
#include "cStealthShip.h"


iShip* cShipFactory::CreateShip(std::string shipType)
{
	iShip* pTheShip = NULL;		// 0

	if ( shipType == "ship")
	{
		pTheShip = new cShip();
	}
	else if (shipType == "fast_ship")
	{
		pTheShip = new cFastShip();
	}
	else if (shipType == "big_fast_ship")
	{
		pTheShip = new cBigFastShip();
	}
	else if ( shipType == "small_ship")
	{
		pTheShip = new cSmallShip();
	}
	else if ( shipType == "stealth_ship_please")
	{
		pTheShip = new cStealthShip();
	}

	// Did I create a ship?
	if (pTheShip != NULL)
	{
		// I did create a ship
		// Pass this to the builder...
		this->BuildShip(pTheShip, shipType );
	}


	return pTheShip;		// NULL or 0
}

void cShipFactory::BuildShip(iShip* pShip, std::string shipType)
{
	if (shipType == "ship")
	{
	//	pShip->SetLASER( new cLASER()) ;
		((cShip*)pShip)->m_pTheLASER = new cLASER();

	}
	else if (shipType == "fast_ship")
	{
		((cFastShip*)pShip)->m_pLASER1 = new cLASER();
		((cFastShip*)pShip)->m_pLASER2 = new cLASER();
	}
	else if (shipType == "big_fast_ship")
	{
		((cBigFastShip*)pShip)->m_pLASER[0] = new cLASER();
		((cBigFastShip*)pShip)->m_pLASER[1] = new cLASER();
		((cBigFastShip*)pShip)->m_pBFL[0] = new cBigFreakingLASER();
		((cBigFastShip*)pShip)->m_pBFL[1] = new cBigFreakingLASER();	

	}
	else if (shipType == "small_ship")
	{
		
	}
	else if (shipType == "stealth_ship_please")
	{
		((cStealthShip*)pShip)->m_pTorpedo = new cTorpedo();

	}

}


void cShipFactory::UpgradeShip(iShip* pShip, std::string upgrade)
{

	if (upgrade == "Ship upgrade to fancy LASER" )
	{
		delete ((cShip*)pShip)->m_pTheLASER;
		((cShip*)pShip)->m_pTheLASER = NULL;

		((cShip*)pShip)->m_pFancyLASER = new cFancyLASER();
	}

	return;
}









iShip* OLD_SCHOOL_CreateShip(std::string shipType)
{
	iShip* pTheShip = NULL;		// 0

	if (shipType == "ship")
	{
		pTheShip = new cShip();
	}
	else if (shipType == "fast_ship")
	{
		pTheShip = new cFastShip();
	}
	else if (shipType == "big_fast_ship")
	{
		pTheShip = new cBigFastShip();
	}
	else if (shipType == "small_ship")
	{
		pTheShip = new cSmallShip();
	}
	else if (shipType == "stealth_ship_please")
	{
		pTheShip = new cStealthShip();
	}


	return pTheShip;		// NULL or 0
}
