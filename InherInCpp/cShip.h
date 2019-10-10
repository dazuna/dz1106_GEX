#pragma once
#include "iShip.h"
#include "iShipDamageInterface.h"
#include "cLASER.h"
#include "cFancyLASER.h"

#include <string>

//#include "iShipBuilder.h"

// Ship has a single LASER

class cShip : public iShip//, public iShipBuilder
{
public:
	cShip();
	virtual ~cShip();
	//virtual void Shoot(iShip* pShipToShoot);
	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Fly(void);				// Only thing that's changing
	virtual void Land(void);

	virtual void TakeDamage(float amount);

	void SomeMethodNooneCallCall(void);

	// From ShipBuilder
//	virtual void SetLASER(cLASER* pTheLASER);
//	virtual void SetFancyLASER(cFancyLASER* pFancyLASER);

	// Danger, Will Robinson, Danger!!
//	friend cShipFactory;

//private:
	cLASER* m_pTheLASER;
	cFancyLASER* m_pFancyLASER;

private:
	std::string HeyThereYouWithTheStarsInYourEyes;
	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 75.0f;


};