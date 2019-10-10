#include "cWorld.h"
#include <iostream>
#include <Windows.h>

//static 
int cWorld::x = 0;

//static 
cWorld* cWorld::m_pTheOneAndOnlyWorldObject = NULL;




//static 
cWorld* cWorld::CreateAWorld_NotThreadSafe(void)
{
	// It's possible that two threads, attempting to create 
	// a singleton object, can hit this method at 
	// the same time, creating two objects, with the 
	// 1st thread getting a different instance. 
	// 
	// Note that this is unlikely, and an alternative
	// is to not use a singleton, and not allow threads
	// to call the factory method at all; instead, you 
	// create the object once, then pass a reference 
	// or pointer to the one object. 


	if (cWorld::m_pTheOneAndOnlyWorldObject == NULL)
	{
		cWorld::m_pTheOneAndOnlyWorldObject = new cWorld();
	}

	return cWorld::m_pTheOneAndOnlyWorldObject;
}



CRITICAL_SECTION CS_FactoryMethod;

//static 
void cWorld::InitCS(void)
{
	InitializeCriticalSection(&CS_FactoryMethod);
	return;
}
//static 
void cWorld::DeleteCS(void)
{
	DeleteCriticalSection(&CS_FactoryMethod);
	return;
}

//static 
cWorld* cWorld::CreateAWorld_ThreadSafe(void)
{
	// Only 1 thread at a time can enter the critical section.
	// The 1st thread to hit this will "lock" this portion
	// of the code, creating the singleton, then exiting.
	// Any later (blocked) thread will then get the 
	// created object.

	// *************************************
	EnterCriticalSection(&CS_FactoryMethod);
	// *************************************

	if (cWorld::m_pTheOneAndOnlyWorldObject == NULL)
	{
		cWorld::m_pTheOneAndOnlyWorldObject = new cWorld();
	}

	// *************************************
	LeaveCriticalSection(&CS_FactoryMethod);
	// *************************************

	return cWorld::m_pTheOneAndOnlyWorldObject;
}

cWorld::cWorld()
{
	std::cout << "A cWorld has been created" << std::endl;
	return;
}


void cWorld::AddShipToWorld(iShip* pTheShip)
{
	this->m_vec_pShips.push_back(pTheShip);

	return;
}