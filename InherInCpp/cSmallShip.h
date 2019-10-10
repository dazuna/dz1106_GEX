#pragma once

#include "iShip.h"
#include "iShipDamageInterface.h"

#include <string>

class cSmallShip : public iShip
{
public:
	cSmallShip();
	virtual ~cSmallShip();

	// From iShip
	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Fly(void);
	virtual void Land(void);

	// From iShipDamageInterface
	virtual void TakeDamage(float amount);

private:
	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 100.0f;

};