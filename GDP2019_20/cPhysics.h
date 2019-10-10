#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include "cGameObject.h"
#include "cMesh.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

class cPhysics
{
public:
	cPhysics();

	// Alias to a type "existing type" "new type name"
	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	struct Sphere
	{
		glm::vec3 c;
		float r;
	};	

	struct sPhysicsTriangle
	{
		glm::vec3 verts[3];
		glm::vec3 normal;
	};

	struct sCollisionInfo
	{
		glm::vec3 closestPoint;
		glm::vec3 directionOfApproach;
		float penetrationDistance;
		glm::vec3 adjustmentVector;
		unsigned int Object1_ID;		// Shpere
		unsigned int Object2_ID;		// Sphere or Triangle
	};
	
	// This "moves" the objects based on the inegration step
	void IntegrationStep( std::vector<cGameObject*>& vec_pGameObjects,
					      float deltaTime );

	void TestForCollisions( std::vector<cGameObject*>& vec_pGameObjects );

	// Returns all the triangles and the closest points
	void GetClosestTriangleToPoint( Point pointXYZ, cMesh &mesh, glm::vec3 &closestPoint, sPhysicsTriangle &closestTriangle );

	// Will return the closest triangles that are within the range "distanceRange".
	// This can be used as a "closest triangles to sphere"
	void GetClosestTrianglesToSphere( cGameObject &testSphere, float distanceRange, cMesh &mesh, std::vector<sPhysicsTriangle> &vecClosestTriangles );


	// Taken from Ericson's book:
	Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);
	int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p);

	void setGravity( glm::vec3 newGravityValue );
	glm::vec3 getGravity(void);

private:

	// Does collision test and returns collision information
	// Returns true if collision, and will load collisionInfo struct
	bool DoSphereSphereCollisionTest( cGameObject* pA, cGameObject *pB, 
									  sCollisionInfo &collisionInfo );
	bool DoShphereMeshCollisionTest( cGameObject* pA, cGameObject* pB,
									 sCollisionInfo &collisionInfo );


	glm::vec3  m_Gravity;

};

#endif
