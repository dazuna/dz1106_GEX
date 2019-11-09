#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include "cGameObject.h"
#include "cMesh.h"
#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>
#include <random>

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
	void IntegrationStep(
		std::map<std::string, cGameObject*> g_map_GameObjects, 
		float deltaTime);

	void TestForCollisions(std::map<std::string, cGameObject*> g_map_GameObjects);

	// Returns all the triangles and the closest points
	void GetClosestTriangleToPoint( 
		Point pointXYZ, cMesh &mesh, glm::vec3 &closestPoint, sPhysicsTriangle &closestTriangle );

	// Will return the closest triangles that are within the range "distanceRange".
	// This can be used as a "closest triangles to sphere"
	void GetClosestTrianglesToSphere( cGameObject &testSphere, float distanceRange, cMesh &mesh, std::vector<sPhysicsTriangle> &vecClosestTriangles );

	// Taken from Ericson's book:
	Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);
	int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p);

	void setGravity( glm::vec3 newGravityValue );
	glm::vec3 getGravity(void);

	// Takes a mesh in "model space" and converts it into "world space"
	void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld,
		cMesh& mesh_transformedInWorld);

	void GetClosestTriangleToPoint_FRAMEWORKS_AND_PATTERNS(
		Point pointXYZ, cMesh& mesh, sPhysicsTriangle& closestTriangle);

	bool DoShphereMeshCollisionTest(cGameObject* pA, cGameObject* pB,
		sCollisionInfo& collisionInfo);

	bool DoShphereMeshCollisionTest(cGameObject* pA, cGameObject* pB);

private:

	// Does collision test and returns collision information
	// Returns true if collision, and will load collisionInfo struct
	bool DoSphereSphereCollisionTest( cGameObject* pA, cGameObject *pB, 
									  sCollisionInfo &collisionInfo );
	
	bool objectsAlreadyCollided(cGameObject* pA, cGameObject* pB,
							std::vector<sCollisionInfo> vecCollisions);
	float get_random(float min, float max);
	glm::vec3  m_Gravity;
	float dampSpeed;
};

#endif
