#pragma once
#include <string>
#include "cShip.h"
#include "iShipDamageInterface.h"
#include "cLASER.h"


class cFastShip : public iShip
{
public:
	cFastShip();
	virtual~cFastShip();

	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Fly(void);
	virtual void Land(void);


	virtual void TakeDamage(float amount);

	cLASER* m_pLASER1;
	cLASER* m_pLASER2;

private:
	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 101.0f;


};