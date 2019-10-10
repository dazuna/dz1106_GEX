#include "cWorld2.h"
#include <iostream>


//static 
// Because this is static, it's created BEFORE 
// the main is called, so is always present. 
// Note: this might be an issue if this object needs other
// objects that aren't created before the main
// Note that this is a stack variable, not a pointer.
cWorld2 cWorld2::theOneAndOnlyWorld;


//static
cWorld2* cWorld2::GetWorld_ThreadSafe(void)
{
	return &(cWorld2::theOneAndOnlyWorld);

//	This might be a little more clear:
//
//	cWorld2* pTheWorld = &theOneAndOnlyWorld;
//
//	return pTheWorld;
}

cWorld2::cWorld2()
{
	std::cout << "A cWorld2 has been created" << std::endl;
	return;
}


void cWorld2::AddShipToWorld(iShip* pTheShip)
{
	this->m_vec_pShips.push_back(pTheShip);

	return;
}