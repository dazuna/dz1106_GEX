#pragma once

class iShipDamageInterface
{
public:
	virtual ~iShipDamageInterface() {};

	// Just so we could shoot each other
	virtual void TakeDamage(float amount) = 0;
};
#pragma once
