#pragma once

#include "cFood.h"

// What the robot sees: 
class iMediatorInterface
{
public:
	virtual ~iMediatorInterface() {};
	// Returns the closest garbage of that type near this location
	virtual cFood findClosestGarbage( eGarbageType, float x, float y ) = 0;

	// Removes the garbage that matches this, if found.
	virtual bool eatGarbage( cFood theGarbage ) = 0;
};