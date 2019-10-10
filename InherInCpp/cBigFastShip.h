#pragma once
#include "cFastShip.h"
#include "iShipDamageInterface.h"
#include "cLASER.h"
#include "cBigFreakingLASER.h"


class cBigFastShip : public iShip
{
public:
	cBigFastShip();
	virtual ~cBigFastShip();

	float GiantFreakingLASER;
	//virtual void Land(void);
	virtual void Fly(void);
	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Land();

	virtual void TakeDamage(float amount);
	
	cLASER* m_pLASER[2];
	cBigFreakingLASER* m_pBFL[2];

private:
	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 100.0f;


};