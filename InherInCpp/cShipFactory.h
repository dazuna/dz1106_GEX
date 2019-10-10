#pragma once

// ONLY include the base interface
// NEVER the actual specific ships
#include "iShip.h"
#include "iShipBuilder.h"
#include <string>
#include <vector>

// FUNCTION
iShip* OLD_SCHOOL_CreateShip(std::string shipType);

// Also is a builder pattern
class cShipFactory
{
public:
	// Returns pointer to ship, or NULL(0) if can't find it
	// Passes a common, known type, like int, string
	// This is the "factory method"
	iShip* CreateShip( std::string shipType );

//	bool CreateShip( iShip* &pTheShip);
//	int CreateShip( iShip* &pTheShip);

	// Another use for the builder
	// "Ship upgrade to fancy LASER"
	void UpgradeShip( iShip* pShip, std::string upgrade );


private:
	// The builder method
	void BuildShip( iShip* pShip, std::string shipType );

};