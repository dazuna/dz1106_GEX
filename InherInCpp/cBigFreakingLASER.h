#pragma once
#include "cLASER.h"

class cBigFreakingLASER : public cLASER
{
public:
	void Fire(void);
	//
	void SetSize(float newSize);
	float GetSize(void);

private:
	float size;
	float energyLeft;
	std::string colour;


};