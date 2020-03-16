#include "cPhysics.h"

#include "cMeshMap.h"
#include "util/tools.h"


cPhysics::cPhysics()
{
	// This is a typical Earth gravity value. 
	// note that this doesn't mean that the simulation will "look good", though... 
//	this->m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	this->m_Gravity = glm::vec3(0.0f, -5.0f, 0.0f);
	this->dampSpeed = 0.99f;
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

		if (pCurObj->inverseMass > 0.0f)
		{
			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )
			pCurObj->accel = this->m_Gravity;
			pCurObj->velocity += pCurObj->accel * deltaTime;
			pCurObj->positionXYZ += pCurObj->velocity * deltaTime;
		}
		else if (pCurObj->inverseMass < 0.0f)
		{
			pCurObj->positionXYZ += pCurObj->velocity * deltaTime;
		}

		if (pCurObj->positionXYZ.y < -55.0f)
		{
			pCurObj->positionXYZ = glm::vec3(get_random(-5.0f,5.0f), 20.0f, 0.0f);
			pCurObj->velocity = glm::vec3(0.0f, -2.0f, 0.0f);
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
		sPlyVertexXYZ_N_UV triVert1 = mesh.vecVertices[curTriangle.vert_index_1];
		sPlyVertexXYZ_N_UV triVert2 = mesh.vecVertices[curTriangle.vert_index_2];
		sPlyVertexXYZ_N_UV triVert3 = mesh.vecVertices[curTriangle.vert_index_3];

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
			
			// Copy the normal, too	
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

	for (std::map<std::string, cGameObject*>::iterator outerLoopIndex = g_map_GameObjects.begin();
		outerLoopIndex != g_map_GameObjects.end(); 
		outerLoopIndex++)
	{
		for (std::map<std::string, cGameObject*>::iterator innerLoopIndex = g_map_GameObjects.begin();
			innerLoopIndex != g_map_GameObjects.end();
			innerLoopIndex++)
		{ 
			cGameObject* pA = outerLoopIndex->second;
			cGameObject* pB = innerLoopIndex->second;

			if ( pA->getUniqueID() == pB->getUniqueID() )
			{	
				continue; // It's the same // Do nothing
			}
			else if(objectsAlreadyCollided(pA,pB,vecCollisions))
			{
				continue;
			}
			else if (pA->physicsShapeType == SPHERE && pB->physicsShapeType == SPHERE)
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
	// get vector from sphere centers
	glm::vec3 ABvec = pB->positionXYZ - pA->positionXYZ;
	glm::vec3 BAvec = pA->positionXYZ - pB->positionXYZ;
	if (glm::length(ABvec) <= 10.0f)
	{
		float correctionDist = 10.0f - glm::length(BAvec);
		glm::vec3 correctionVector = glm::normalize(BAvec) * correctionDist;
		pA->positionXYZ = pA->positionXYZ + correctionVector;

		//glm::vec3 velocityVector = glm::normalize(pA->velocity);
		//glm::vec3 reflectionVec = glm::reflect(velocityVector, glm::normalize(BAvec));
		//reflectionVec = glm::normalize(reflectionVec);
		//float speed = glm::length(pA->velocity);
		//pA->velocity = reflectionVec * speed * dampSpeed;
		//glm::vec3 velocityVectorB = glm::normalize(pB->velocity);
		//glm::vec3 reflectionVecB = glm::reflect(velocityVectorB, glm::normalize(ABvec));
		//reflectionVecB = glm::normalize(reflectionVecB);
		//float speedB = glm::length(pB->velocity);
		//pB->velocity = reflectionVecB * speedB * dampSpeed;

		pA->objectColourRGBA = glm::vec4(get_random(0.0,1.0), get_random(0.0, 1.0), get_random(0.0, 1.0),1.0f);
		pB->objectColourRGBA = glm::vec4(get_random(0.0, 1.0), get_random(0.0, 1.0), get_random(0.0, 1.0), 1.0f);

		std::cout << "pA: " << glm::to_string(pA->positionXYZ) << std::endl;
		std::cout << "pB: " << glm::to_string(pB->positionXYZ) << std::endl;
		std::cout << "AB: " << glm::to_string(ABvec) << std::endl;
		std::cout << "BA: " << glm::to_string(BAvec) << std::endl;
		std::cout << "endl"<< std::endl;
		collisionInfo.Object1_ID = pA->getUniqueID();
		collisionInfo.Object2_ID = pB->getUniqueID();
		return true;
	}
	return false;
}
bool cPhysics::DoShphereMeshCollisionTest(cGameObject* pSphere, cGameObject* pB,
								sCollisionInfo& collisionInfo)
{
	//**********************************************************
	// 	Collisions in World
	//**********************************************************
	//cGameObject* pSphere = ::g_map_GameObjects["sphere"];
	glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	cPhysics::sPhysicsTriangle closestTriangle;
	glm::mat4 matWorld = tools::calculateWorldMatrix(pB);
	cMesh transMesh;
	cMeshMap* theMeshMap = cMeshMap::getTheMeshMap();
	//CalculateTransformedMesh(*::g_map_Mesh[pB->meshName.c_str()], matWorld, transMesh);
	CalculateTransformedMesh(*(theMeshMap->getMesh(pB->meshName)), matWorld, transMesh);
	GetClosestTriangleToPoint(pSphere->positionXYZ, transMesh, closestPoint, closestTriangle);

	// Highlight the triangle that I'm closest to
	//pDebugRenderer->addTriangle(closestTriangle.verts[0],
	//	closestTriangle.verts[1],
	//	closestTriangle.verts[2],
		//glm::vec3(1.0f, 0.0f, 0.0f));

	// Highlight the triangle that I'm closest to
	// To draw the normal, calculate the average of the 3 vertices, 
	// then draw that average + the normal (the normal starts at the 0,0,0 OF THE TRIANGLE)
	glm::vec3 centreOfTriangle = (closestTriangle.verts[0] +
		closestTriangle.verts[1] +
		closestTriangle.verts[2]) / 3.0f;		// Average

	glm::vec3 normalInWorld = centreOfTriangle + (closestTriangle.normal * 20.0f);	// Normal x 10 length

	//pDebugRenderer->addLine(centreOfTriangle,
	//	normalInWorld,
	//	glm::vec3(1.0f, 1.0f, 0.0f));

	// Are we hitting the triangle? 
	float distance = glm::length(pSphere->positionXYZ - closestPoint);
	//std::cout << pSphere->friendlyName << " dist: " << distance << std::endl;
	if (distance < 3.f)
	{
		// 1. Calculate vector from centre of sphere to closest point
		glm::vec3 vecSphereToClosestPoint = closestPoint - pSphere->positionXYZ;
		// 2. Get the length of this vector
		float centreToContractDistance = glm::length(vecSphereToClosestPoint);
		// 3. Create a vector from closest point to radius
		float lengthPositionAdjustment = (4.f) - centreToContractDistance;
		// 4. Sphere is moving in the direction of the velocity, so 
		//    we want to move the sphere BACK along this velocity vector
		if(glm::length(pSphere->velocity) < 1.f) { return true; }
		glm::vec3 vecDirection = glm::normalize(pSphere->velocity);
		glm::vec3 vecPositionAdjust = (-vecDirection) * lengthPositionAdjustment;
		// 5. Reposition sphere 
		pSphere->positionXYZ += (vecPositionAdjust);

		//// Is in contact with the triangle... 
		//// Calculate the response vector off the triangle. 
		//glm::vec3 velocityVector = glm::normalize(pSphere->velocity);

		//// closestTriangle.normal
		//glm::vec3 reflectionVec = glm::reflect(velocityVector, glm::normalize(closestTriangle.normal));
		//reflectionVec = glm::normalize(reflectionVec);

		////// Stop the sphere and draw the two vectors...
		//////pSphere->inverseMass = 0.0f;	// Stopped
		////glm::vec3 velVecX20 = velocityVector * 10.0f;
		////pDebugRenderer->addLine(closestPoint, velVecX20,
		////	glm::vec3(1.0f, 0.0f, 0.0f), 30.0f /*seconds*/);
		////glm::vec3 reflectionVecX20 = reflectionVec * 10.0f;
		////pDebugRenderer->addLine(closestPoint, reflectionVecX20,
		////	glm::vec3(0.0f, 1.0f, 1.0f), 30.0f /*seconds*/);

		//// Change the direction of the ball (the bounce off the triangle)
		//// Get lenght of the velocity vector
		//float speed = glm::length(pSphere->velocity);
		//pSphere->velocity = reflectionVec * speed * dampSpeed;
		////std::cout << "velocityVector: " << GLMvec3toString(velocityVector) << std::endl;
		////std::cout << "reflectionVec: " << GLMvec3toString(reflectionVec) << std::endl;
		////std::cout << "positionXYZ: " << GLMvec3toString(pSphere->positionXYZ) << std::endl;
		////std::cout << "triNormal: " << GLMvec3toString(closestTriangle.normal) << std::endl;
		////std::cout << "endl" << std::endl;
		return true;
	}
	return false;
}

bool cPhysics::DoShphereMeshCollisionTest(cGameObject* pSphere, cGameObject* pB)
{
	//**********************************************************
	// 	Collisions in World
	//**********************************************************
	//cGameObject* pSphere = ::g_map_GameObjects["sphere"];
	glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	cPhysics::sPhysicsTriangle closestTriangle;
	glm::mat4 matWorld = tools::calculateWorldMatrix(pB);
	cMesh transMesh;
	cMeshMap* theMeshMap = cMeshMap::getTheMeshMap();
	//CalculateTransformedMesh(*::g_map_Mesh[pB->meshName.c_str()], matWorld, transMesh);
	CalculateTransformedMesh(*(theMeshMap->getMesh(pB->meshName)), matWorld, transMesh);
	GetClosestTriangleToPoint(pSphere->positionXYZ, transMesh, closestPoint, closestTriangle);

	// Highlight the triangle that I'm closest to
	//pDebugRenderer->addTriangle(closestTriangle.verts[0],
	//	closestTriangle.verts[1],
	//	closestTriangle.verts[2],
		//glm::vec3(1.0f, 0.0f, 0.0f));

	// Highlight the triangle that I'm closest to
	// To draw the normal, calculate the average of the 3 vertices, 
	// then draw that average + the normal (the normal starts at the 0,0,0 OF THE TRIANGLE)
	glm::vec3 centreOfTriangle = (closestTriangle.verts[0] +
		closestTriangle.verts[1] +
		closestTriangle.verts[2]) / 3.0f;		// Average

	glm::vec3 normalInWorld = centreOfTriangle + (closestTriangle.normal * 20.0f);	// Normal x 10 length

	//pDebugRenderer->addLine(centreOfTriangle,
	//	normalInWorld,
	//	glm::vec3(1.0f, 1.0f, 0.0f));

	// Are we hitting the triangle? 
	float distance = glm::length(pSphere->positionXYZ - closestPoint);

	if (distance <= (pSphere->scale / 2))
	{
		return true;
	}
	return false;
}

// Takes a mesh in "model space" and converts it into "world space"
void cPhysics::CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld,
	cMesh& mesh_transformedInWorld)
{
	// Make a copy of the mesh...
	mesh_transformedInWorld = originalMesh;

	// Using the same thing that happens in the shader, 
	// we transform the vertices of the mesh by the world matrix
	// fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);

	for (std::vector<sPlyVertexXYZ_N_UV>::iterator itVert = mesh_transformedInWorld.vecVertices.begin();
		itVert != mesh_transformedInWorld.vecVertices.end(); itVert++)
	{
		glm::vec4 vertex = glm::vec4(itVert->x, itVert->y, itVert->z, 1.0f);

		// boom goes the dynamite
		// https://www.youtube.com/watch?v=W45DRy7M1no
		glm::vec4 vertexWorldTransformed = matWorld * vertex;

		// Update 
		itVert->x = vertexWorldTransformed.x;
		itVert->y = vertexWorldTransformed.y;
		itVert->z = vertexWorldTransformed.z;

		// CALCAULTE THE NORMALS for the this mesh, too (for the response)
		// for the normal, do the inverse transpose of the world matrix
		glm::mat4 matWorld_Inv_Transp = glm::inverse(glm::transpose(matWorld));
		glm::vec4 normal = glm::vec4(itVert->nx, itVert->ny, itVert->nz, 1.0f);

		// boom goes the dynamite
		// https://www.youtube.com/watch?v=W45DRy7M1no
		glm::vec4 normalWorldTransformed = matWorld_Inv_Transp * normal;

		// Update 
		itVert->nx = normalWorldTransformed.x;
		itVert->ny = normalWorldTransformed.y;
		itVert->nz = normalWorldTransformed.z;
	}

	return;
}

bool cPhysics::objectsAlreadyCollided(cGameObject* pA, cGameObject* pB, 
							std::vector<sCollisionInfo> vecCollisions)
{
	for (unsigned int i = 0; i < vecCollisions.size(); i++)
	{
		if (vecCollisions[i].Object1_ID == pA->getUniqueID() &&
			vecCollisions[i].Object2_ID == pB->getUniqueID())
		{
			return true;
		}
		if (vecCollisions[i].Object2_ID == pA->getUniqueID() &&
			vecCollisions[i].Object1_ID == pB->getUniqueID())
		{
			return true;
		}
	}
	return false;
}

float cPhysics::get_random(float min,float max)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(min, max); // rage 0 - 1
	return dis(e);
}

// *********************************************************************************
// FOR THE PATTERNS AND FRAMEWORKS MID-TERM
// 
// This is like the method above, but doesn't test for the y axis, which gives you the 
// TRIANGLE (not the point) that that point is over (or under). 
// 
void cPhysics::GetClosestTriangleToPoint_FRAMEWORKS_AND_PATTERNS(Point pointXYZ, cMesh& mesh, sPhysicsTriangle& closestTriangle)
{
	// Assume the closest distance is REALLY far away
	float closestDistanceSoFar = FLT_MAX;


	for (unsigned int triIndex = 0;
		 triIndex != mesh.vecTriangles.size();
		 triIndex++)
	{
		sPlyTriangle& curTriangle = mesh.vecTriangles[triIndex];

		// Get the vertices of the triangle
		sPlyVertexXYZ_N_UV triVert1 = mesh.vecVertices[curTriangle.vert_index_1];
		sPlyVertexXYZ_N_UV triVert2 = mesh.vecVertices[curTriangle.vert_index_2];
		sPlyVertexXYZ_N_UV triVert3 = mesh.vecVertices[curTriangle.vert_index_3];

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


		// Now clear the y values for the actual test
		glm::vec3 triVertPoint1_y_is_zero = triVertPoint1;
		glm::vec3 triVertPoint2_y_is_zero = triVertPoint2;
		glm::vec3 triVertPoint3_y_is_zero = triVertPoint3;
		glm::vec3 testPoint_y_is_zero = pointXYZ;


		triVertPoint1_y_is_zero.y = 0.0f;
		triVertPoint2_y_is_zero.y = 0.0f;
		triVertPoint3_y_is_zero.y = 0.0f;
		testPoint_y_is_zero.y = 0.0f;

		glm::vec3 curClosetPoint = ClosestPtPointTriangle(testPoint_y_is_zero,
														  triVertPoint1_y_is_zero, triVertPoint2_y_is_zero, triVertPoint3_y_is_zero);

		// Is this the closest so far?
		float distanceNow = glm::distance(curClosetPoint, pointXYZ);

		// is this closer than the closest distance
		if (distanceNow <= closestDistanceSoFar)
		{
			closestDistanceSoFar = distanceNow;

			// Copy the triangle information...
			closestTriangle.verts[0].x = triVert1.x;
			closestTriangle.verts[0].y = triVert1.y;
			closestTriangle.verts[0].z = triVert1.z;
			closestTriangle.verts[1].x = triVert2.x;
			closestTriangle.verts[1].y = triVert2.y;
			closestTriangle.verts[1].z = triVert2.z;
			closestTriangle.verts[2].x = triVert3.x;
			closestTriangle.verts[2].y = triVert3.y;
			closestTriangle.verts[2].z = triVert3.z;
		}
	}//for (unsigned int triIndex = 0;
	return;
}
// *********************************************************************************