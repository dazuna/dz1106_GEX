#include "cPhysics.h"

cPhysics::cPhysics()
{
	// This is a typical Earth gravity value. 
	// note that this doesn't mean that the simulation will "look good", though... 
//	this->m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	this->m_Gravity = glm::vec3(0.0f, -1.0f, 0.0f);
	return;
}


void cPhysics::setGravity(glm::vec3 newGravityValue)
{
	this->m_Gravity = newGravityValue;
	return;
}

glm::vec3 cPhysics::getGravity(void)
{
	return this->m_Gravity;
}


void cPhysics::IntegrationStep(std::map<std::string, cGameObject*> g_map_GameObjects, float deltaTime)
{
	for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects.begin();
		itGO != g_map_GameObjects.end();
		itGO++)
	{
		// Get a pointer to the current object (makes the code a little clearer)
		cGameObject* pCurObj = itGO->second;

		if (pCurObj->inverseMass != 0.0f)
		{
			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )
			pCurObj->accel = this->m_Gravity;
			pCurObj->velocity += pCurObj->accel * deltaTime;
			pCurObj->positionXYZ += pCurObj->velocity * deltaTime;
		}
	}//for (unsigned int index = 0;

	return;
}

// Returns all the triangles and the closest points
void cPhysics::GetClosestTriangleToPoint(Point pointXYZ, cMesh& mesh, 
	glm::vec3& closestPoint, sPhysicsTriangle& closestTriangle)
{

	// Assume the closest distance is REALLY far away
	float closestDistanceSoFar = FLT_MAX;


	for (unsigned int triIndex = 0;
		 triIndex != mesh.vecTriangles.size();
		 triIndex++)
	{
		sPlyTriangle& curTriangle = mesh.vecTriangles[triIndex];

		// Get the vertices of the triangle
		sPlyVertexXYZ_N triVert1 = mesh.vecVertices[curTriangle.vert_index_1];
		sPlyVertexXYZ_N triVert2 = mesh.vecVertices[curTriangle.vert_index_2];
		sPlyVertexXYZ_N triVert3 = mesh.vecVertices[curTriangle.vert_index_3];

		Point triVertPoint1;
		triVertPoint1.x = triVert1.x;
		triVertPoint1.y = triVert1.y;
		triVertPoint1.z = triVert1.z;

		Point triVertPoint2;
		triVertPoint2.x = triVert2.x;
		triVertPoint2.y = triVert2.y;
		triVertPoint2.z = triVert2.z;

		Point triVertPoint3;
		triVertPoint3.x = triVert3.x;
		triVertPoint3.y = triVert3.y;
		triVertPoint3.z = triVert3.z;

		glm::vec3 curClosetPoint = ClosestPtPointTriangle( pointXYZ,
														   triVertPoint1, triVertPoint2, triVertPoint3);

		// Is this the closest so far?
		float distanceNow = glm::distance(curClosetPoint, pointXYZ);

		// is this closer than the closest distance
		if (distanceNow <= closestDistanceSoFar)
		{
			closestDistanceSoFar = distanceNow;

			closestPoint = curClosetPoint;

			// Copy the triangle information over, as well
			closestTriangle.verts[0].x = triVert1.x;
			closestTriangle.verts[0].y = triVert1.y;
			closestTriangle.verts[0].z = triVert1.z;
			closestTriangle.verts[1].x = triVert2.x;
			closestTriangle.verts[1].y = triVert2.y;
			closestTriangle.verts[1].z = triVert2.z;
			closestTriangle.verts[2].x = triVert3.x;
			closestTriangle.verts[2].y = triVert3.y;
			closestTriangle.verts[2].z = triVert3.z;
			
			// TODO: Copy the normal, too	
			// Quick is to average the normal of all 3 vertices
			glm::vec3 triVert1Norm = glm::vec3(triVert1.nx, triVert1.ny, triVert1.nz );
			glm::vec3 triVert2Norm = glm::vec3(triVert2.nx, triVert2.ny, triVert2.nz );
			glm::vec3 triVert3Norm = glm::vec3(triVert3.nx, triVert3.ny, triVert3.nz );

			// Average of the vertex normals... 
			closestTriangle.normal = (triVert1Norm + triVert2Norm + triVert3Norm) / 3.0f;

		}

	}//for (unsigned int triIndex = 0;

	return;
}

// Will return the closest triangles that are within the range "distanceRange".
// This can be used as a "closest triangles to sphere"
void cPhysics::GetClosestTrianglesToSphere(cGameObject& testSphere, float distanceRange, cMesh& mesh, std::vector<sPhysicsTriangle>& vecClosestTriangles)
{



}

// Test each object with every other object
void cPhysics::TestForCollisions(std::map<std::string, cGameObject*> g_map_GameObjects)
{
	// This will store all the collisions in this frame
	std::vector<sCollisionInfo> vecCollisions;

	sCollisionInfo collisionInfo;

	for (unsigned int outerLoopIndex = 0;
		 outerLoopIndex != vec_pGameObjects.size(); outerLoopIndex++)
	{
		for (unsigned int innerLoopIndex = 0;
			 innerLoopIndex != vec_pGameObjects.size(); innerLoopIndex++)
		{ 
			cGameObject* pA = vec_pGameObjects[outerLoopIndex];
			cGameObject* pB = vec_pGameObjects[innerLoopIndex];

			if ( pA->getUniqueID() == pB->getUniqueID() )
			{	
				return; // It's the same // Do nothing
			}
			else if (pA->physicsShapeType == SPHERE &&
				pB->physicsShapeType == SPHERE)
			{
				if (DoSphereSphereCollisionTest(pA, pB, collisionInfo))
				{
					vecCollisions.push_back(collisionInfo);
				}
			}
			else if (pA->physicsShapeType == SPHERE &&
					 pB->physicsShapeType == MESH)
			{
				if (DoShphereMeshCollisionTest(pA, pB, collisionInfo))
				{
					vecCollisions.push_back(collisionInfo);
				}
			}
		}//for (unsigned int innerLoopIndex = 0;
	}//for (unsigned int outerLoopIndex = 0;

}

bool cPhysics::DoSphereSphereCollisionTest(cGameObject* pA, cGameObject* pB,
								 sCollisionInfo& collisionInfo)
{
	// TODO: 
	// Run the sphere-sphere collision test
	// If collided, load the collisionInfo struct and return true
	// else return false;

	return false;
}
bool cPhysics::DoShphereMeshCollisionTest(cGameObject* pA, cGameObject* pB,
								sCollisionInfo& collisionInfo)
{
	// TODO: Do the sphere-Mesh collision test
	// If collided, load the collisionInfo struct and return true
	//  else return false


	return true;
}
