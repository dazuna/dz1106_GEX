#pragma once
#include "iShip.h"
#include <vector>


class cWorld2
{
public:
//	cWorld();		// Now private
	void AddShipToWorld( iShip* pTheShip );

	// or automatic thread safe...
	static cWorld2* GetWorld_ThreadSafe(void);


private:

	// remember what ships we made
	std::vector< iShip* > m_vec_pShips;

	// Move constructor into private
	cWorld2();

	// Because this is static, it's created BEFORE 
	// the main is called, so is always present. 
	// Note: this might be an issue if this object needs other
	// objects that aren't created before the main
	// Note that this is a stack variable, not a pointer.
	static cWorld2 theOneAndOnlyWorld;
};