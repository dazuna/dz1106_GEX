#include "cStealthShip.h"

#include <iostream>

cStealthShip::cStealthShip()
{
	std::cout << "a cStealthShip is created (but you can't see it)" << std::endl;
}
cStealthShip::~cStealthShip()
{
	std::cout << "a cStealthShip is destroyed" << std::endl;
}


void cStealthShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cStealthShip: " << this->pilotName << " Shoots " << std::endl;
	// 

	this->m_pTorpedo->Woosh();


	pShipToShoot->TakeDamage(10.0f);

	//dynamic_cast<iShip*>(pShipToShoot)->Fly();

	return;
}

void cStealthShip::Fly(void)
{
	std::cout << "cStealthShip: " << this->pilotName << " Flies" << std::endl;
	return;
}

void cStealthShip::Land(void)
{
	std::cout << "cStealthShip lands" << std::endl;
}

void cStealthShip::TakeDamage(float amount)
{
	std::cout << "cStealthShip " << this->pilotName <<
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}