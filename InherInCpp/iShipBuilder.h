#pragma once
#include "cLASER.h"
#include "cFancyLASER.h"


class iShipBuilder
{
public:
	virtual ~iShipBuilder() {};

	virtual void SetLASER(cLASER* pTheLASER) = 0;
	virtual void SetFancyLASER(cFancyLASER* pFancyLASER) = 0;

};