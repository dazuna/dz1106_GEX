#include "cAABB/cAABB.h"

// Test to see where our player (ship, whatever) is (which AABB it's in)
static unsigned long long get_ID_of_AABB_I_Might_Be_In(glm::vec3 testLocation)
{
	return cAABB::calcID(testLocation);
}

static unsigned long long calcID(glm::vec3 testLocation)
{
	unsigned long long ID;		// x, y, z

		// Make sure these are rounded to the nearest 100 (length)
	unsigned long long ulX = 0;
	unsigned long long ulY = 0;
	unsigned long long ulZ = 0;

	// Are these negative
	if (testLocation.x < 0.0f)
	{
		ulX += 100000;		// 000,000   100,000   100300
	}
	if (testLocation.y < 0.0f)
	{
		ulY += 100000;		// 000,000   100,000
	}
	if (testLocation.z < 0.0f)
	{
		ulZ += 100000;		// 000,000   100,000
	}

	ulX = (((unsigned long long) fabs(testLocation.x)) / 100) + 100;	// -300	
	ulY = (((unsigned long long) fabs(testLocation.y)) / 100) + 100;	// 700
	ulZ = (((unsigned long long) fabs(testLocation.z)) / 100) + 100;	// -2400

	unsigned long long oneMillion = 1000000;

	ulX *= oneMillion * oneMillion;			// 100 * 100
	ulY *= oneMillion;
	ulZ *= 1;

	ID = ulX + ulY + ulZ;

	return ID;


}

// NOTE: it's unsigned and it's 64 bit
// This returns the ID the specific AABB
unsigned long long getID(void)
{
	unsigned long long ID = cAABB::calcID(this->minXYZ);		// x, y, z
	return ID;
}


bool isVertInside(glm::vec3 VertexXYZ)
{
	if (VertexXYZ.x < minXYZ.x)	return false;
	if (VertexXYZ.x > (minXYZ.x + length)) return false;
	if (VertexXYZ.y < minXYZ.y)	return false;
	if (VertexXYZ.y > (minXYZ.y + length)) return false;
	if (VertexXYZ.z < minXYZ.z)	return false;
	if (VertexXYZ.z > (minXYZ.z + length)) return false;

	// It's inside!

};