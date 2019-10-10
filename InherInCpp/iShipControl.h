#pragma once
#include "iShipDamageInterface.h"

// "Interface"class (ALL pure virtual)
// DON'T MAKE A NON PURE VIRTUAL INTERFACE CLASS
class iShipControl
{
public:
	virtual ~iShipControl() {};		// HAVE TO DO IT
//	virtual void Shoot(iShip* pShipToShoot) = 0;	// Pure virtual
	virtual void Shoot(iShipDamageInterface* pShipToShoot) = 0;	// Pure virtual
	virtual void Fly(void) = 0;
	virtual void Land(void) = 0;

};
