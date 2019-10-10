#ifndef _cWorld_HG_
#define _cWorld_HG_

#include "iShip.h"
#include <vector>

// One version of the singleton. 
// In this one, a pointer is kept, and when the 
// 'factory' method is called, we check if the pointer
// actually points to anything, yet. If not, we create 
// the object. 
// If it does point to something, we return that object.

class cWorld
{
public:
//	cWorld();		// Now private
	void AddShipToWorld( iShip* pTheShip );

	static cWorld* CreateAWorld_NotThreadSafe(void);

	// We would have to call this 1st, by one thread
	static void InitCS(void);
	static cWorld* CreateAWorld_ThreadSafe(void);
	static void DeleteCS(void);
	

	static int x;
private:

	// remember what ships we made
	std::vector< iShip* > m_vec_pShips;

	// Move constructor into private
	// Now, it can't be called. 
	cWorld();

	// static variable means there's only one of them
	// shared will ALL the classes. 
	static cWorld* m_pTheOneAndOnlyWorldObject;
};

#endif  
