#include "cSmallShip.h"
#include <iostream>

cSmallShip::cSmallShip()
{
	std::cout << "a cSmallShip is created" << std::endl;
}
cSmallShip::~cSmallShip()
{
	std::cout << "a cSmallShip is destroyed" << std::endl;
}

void cSmallShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cSmallShip::Shoot()" << std::endl;

	return;
}

void cSmallShip::Fly(void)
{
	std::cout << "cSmallShip::Fly()" << std::endl;
	return;
}

void cSmallShip::Land(void)
{
	std::cout << "cSmallShip lands" << std::endl;
}


void cSmallShip::TakeDamage(float amount)
{
	std::cout << "cSmallShip " << this->pilotName <<
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}