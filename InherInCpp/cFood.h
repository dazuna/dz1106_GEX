#pragma once

enum eGarbageType
{
	ALUMINUM,
	STEEL,
	PLASTIC,
	ELECTRONICS, 
	UNKNOWN
};

class cFood
{
public:
	cFood() : type(UNKNOWN), amount(0.0f), x(0.0f), y(0.0f) {};
	eGarbageType type;
	float amount;
	float x, y;		// Location in the world
};