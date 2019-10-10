#pragma once

#pragma once

#include <iostream>

class cFancyLASER
{
public:
	void Fire(void);

	void SetSize(float newSize);
	float GetSize(void);

protected:		// private but public inherited can see it
	float size;
	float energyLeft;
	std::string colour;
};