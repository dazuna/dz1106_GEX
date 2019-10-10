#include "cShip.h"

#include <iostream>

cShip::cShip()
{
	std::cout << "a CShip is created" << std::endl;

	this->m_pTheLASER = NULL;
	this->m_pFancyLASER = NULL;
}

cShip::~cShip()
{
	std::cout << "a CShip is destroyed" << std::endl;
}


void cShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cShip: " << this->pilotName << " Shoots " << std::endl;
	// 
	if (this->m_pTheLASER)		// If pTheLASER != NULL
	{
		this->m_pTheLASER->Fire();
	}
	if (this->m_pFancyLASER)
	{
		this->m_pFancyLASER->Fire();
	}

	pShipToShoot->TakeDamage(10.0f);

	//dynamic_cast<iShip*>(pShipToShoot)->Fly();

	return;
}

void cShip::Fly(void)
{
	std::cout<< "cShip: " <<  this->pilotName << " Flies" << std::endl;
	return;
}

void cShip::Land(void)
{
	std::cout << "cShip lands" << std::endl;
}

void cShip::TakeDamage(float amount)
{
	std::cout << "cShip " << this->pilotName << 
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}
//
//void cShip::SetLASER(cLASER* pTheLASER)
//{
//	this->m_pTheLASER = pTheLASER;
//}
//
//void cShip::SetFancyLASER(cFancyLASER* pFancyLASER)
//{
//	this->m_pFancyLASER = pFancyLASER;
//}
