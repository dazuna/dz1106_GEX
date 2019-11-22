#include "PhysicsAABBStuff.h"
#include "../playerController/playerController.h"

// This will hold the AABBs for 
std::map<unsigned long long /*ID*/, cAABB*> g_mapAABBs_World;
const float AABBsize = 150.0f;
extern playerController* pPlayerControl;

// Take the space station model
// Figure out which triangles are in which AABB
void CalcAABBsForMeshModel(cMesh* theMesh)
{
	// For the space station, I'll make a size of -5,000 to 5,000
	const float WorldSize = 900.0f;
	
	// XYZ for that generates the cool boxes :D
	for (float x = -WorldSize; x <= WorldSize; x += AABBsize)
	{
		for (float y = -WorldSize; y <= WorldSize; y += AABBsize)
		{
			for (float z = -WorldSize; z <= WorldSize; z += AABBsize)
			{
				cAABB* pAABB = new cAABB();
				pAABB->minXYZ.x = x;
				pAABB->minXYZ.y = y;
				pAABB->minXYZ.z = z;
				pAABB->length = AABBsize;

				unsigned long long ID = pAABB->getID();
				::g_mapAABBs_World[ID] = pAABB;
			}
		}
	}

	// Check which AABB this triangle is in
	for (std::vector<AABBTriangle>::iterator itTri = theMesh->vecABTriangles.begin();
		itTri != theMesh->vecABTriangles.end(); itTri++)
	{
		insertTriangleIntoAABBs(&(*itTri));
		triforceTheTriangle(*itTri, &(*itTri));
	}

	for (std::map<unsigned long long, cAABB*>::iterator itM_AB = ::g_mapAABBs_World.begin();
		itM_AB != ::g_mapAABBs_World.end(); itM_AB++)
	{
		if (itM_AB->second->vecABTriangles.size() == 0)
		{
			::g_mapAABBs_World.erase(itM_AB->first);
		}
	}
	return;
}

void drawAABBAndTrisWherePlayerPresent(cGameObject* pPlayer)
{
	unsigned long long playerAABB_ID = cAABB::getIDAABB8(pPlayer->positionXYZ);
	std::map<unsigned long long, cAABB* >::iterator itAABB = ::g_mapAABBs_World.find(playerAABB_ID);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		return;
	}
	else
	{
		//system("cls");
		//std::cout << "Player is inside AABB: " << playerAABB_ID << "!!" << std::endl;
		itAABB->second->drawAABBSelf(pDebugRenderer);
		itAABB->second->drawAABBSelfTriangles(pDebugRenderer);
	}
}

void insertTriangleIntoAABBs(AABBTriangle* ABTri)
{
	unsigned long long ID_AABB_V1  = cAABB::getIDAABB8(ABTri->a);
	unsigned long long ID_AABB_V2  = cAABB::getIDAABB8(ABTri->b);
	unsigned long long ID_AABB_V3  = cAABB::getIDAABB8(ABTri->c);

	std::map<unsigned long long, cAABB* >::iterator itAABB = ::g_mapAABBs_World.find(ID_AABB_V1);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V1 <<"apparently not in any AABB created " << glm::to_string(ABTri->a) << std::endl;
		return;
	}
	itAABB = ::g_mapAABBs_World.find(ID_AABB_V2);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V2 << "apparently not in any AABB created " << glm::to_string(ABTri->b) << std::endl;
		return;
	}
	itAABB = ::g_mapAABBs_World.find(ID_AABB_V3);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V3 << "apparently not in any AABB created " << glm::to_string(ABTri->c) << std::endl;
		return;
	}

	::g_mapAABBs_World[ID_AABB_V1]->vecABTriangles.insert(ABTri);
	::g_mapAABBs_World[ID_AABB_V2]->vecABTriangles.insert(ABTri);
	::g_mapAABBs_World[ID_AABB_V3]->vecABTriangles.insert(ABTri);
}

void insertTriangleIntoAABBs(AABBTriangle* ABTri, AABBTriangle* OGTri)
{
	unsigned long long ID_AABB_V1 = cAABB::getIDAABB8(ABTri->a);
	unsigned long long ID_AABB_V2 = cAABB::getIDAABB8(ABTri->b);
	unsigned long long ID_AABB_V3 = cAABB::getIDAABB8(ABTri->c);

	std::map<unsigned long long, cAABB* >::iterator itAABB = ::g_mapAABBs_World.find(ID_AABB_V1);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V1 << "apparently not in any AABB created " << glm::to_string(ABTri->a) << std::endl;
		return;
	}
	itAABB = ::g_mapAABBs_World.find(ID_AABB_V2);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V2 << "apparently not in any AABB created " << glm::to_string(ABTri->b) << std::endl;
		return;
	}
	itAABB = ::g_mapAABBs_World.find(ID_AABB_V3);
	if (itAABB == ::g_mapAABBs_World.end())
	{
		std::cout << ID_AABB_V3 << "apparently not in any AABB created " << glm::to_string(ABTri->c) << std::endl;
		return;
	}

	::g_mapAABBs_World[ID_AABB_V1]->vecABTriangles.insert(OGTri);
	::g_mapAABBs_World[ID_AABB_V2]->vecABTriangles.insert(OGTri);
	::g_mapAABBs_World[ID_AABB_V3]->vecABTriangles.insert(OGTri);
}

void triforceTheTriangle(AABBTriangle theTriforce, AABBTriangle* theOGTriangle)
{
	float AABBlength = AABBsize;
	float ab, bc, ca;
	ab = glm::distance(theTriforce.a, theTriforce.b);
	bc = glm::distance(theTriforce.b, theTriforce.c);
	ca = glm::distance(theTriforce.c, theTriforce.a);

	if ((ab > AABBlength) || (bc > AABBlength) || (ca > AABBlength))
	{
		glm::vec3 abMid, bcMid, caMid;
		abMid = ((theTriforce.a + theTriforce.b)/2.0f);
		bcMid = ((theTriforce.b + theTriforce.c)/2.0f);
		caMid = ((theTriforce.c + theTriforce.a)/2.0f);
		//center
		AABBTriangle tmpTri;
		tmpTri.a = abMid;
		tmpTri.b = bcMid;
		tmpTri.c = caMid;		
		insertTriangleIntoAABBs(&tmpTri, theOGTriangle);
		triforceTheTriangle(tmpTri, theOGTriangle);
		// firstCW
		tmpTri.a = theTriforce.a;
		tmpTri.b = abMid;
		tmpTri.c = caMid;
		triforceTheTriangle(tmpTri, theOGTriangle);
		// secondCW
		tmpTri.a = theTriforce.b;
		tmpTri.b = abMid;
		tmpTri.c = bcMid;
		triforceTheTriangle(tmpTri, theOGTriangle);
		// thirdCW
		tmpTri.a = theTriforce.c;
		tmpTri.b = caMid;
		tmpTri.c = bcMid;
		triforceTheTriangle(tmpTri, theOGTriangle);
	}
	return;
}

void positionPlayerColliders(std::string objsFriendlyName)
{
	std::cout << "creating collidable object... " << objsFriendlyName << std::endl;
	std::map<std::string, cGameObject*>::iterator itGO = ::g_map_GameObjects.find(objsFriendlyName);
	if (itGO == ::g_map_GameObjects.end())
	{
		std::cout << "couldn't find gameObject: "<< objsFriendlyName << std::endl;
		return;
	}
	cGameObject* pPlayer = itGO->second;
	glm::vec3 origin = pPlayer->positionXYZ;
	std::vector<glm::vec3> vecColls;

	vecColls.push_back(glm::vec3(-7.3, 0.0, -9.2));
	vecColls.push_back(glm::vec3(7.3, 0.0, -9.2));
	vecColls.push_back(glm::vec3(-7.3, 2.4, 9.2));
	vecColls.push_back(glm::vec3(7.3, 2.4, 9.2));
	vecColls.push_back(glm::vec3(-7.3, -2.4, 9.2));
	vecColls.push_back(glm::vec3(7.3, -2.4, 9.2));
	vecColls.push_back(glm::vec3(3.9, 6.0, -3.0));
	vecColls.push_back(glm::vec3(-3.9, 6.0, -3.0));
	vecColls.push_back(glm::vec3(3.9, -6.0, -3.0));
	vecColls.push_back(glm::vec3(-3.9, -6.0, -3.0));

	pPlayer->setVecCollider(vecColls);
}

void drawPlayerColliders(cGameObject* pPlayer)
{
	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);

	glm::vec3 origin = pPlayer->positionXYZ;
	std::vector<glm::vec3>::iterator itVC;
	std::vector<glm::vec3> vecColls;
	pPlayer->getVecColliders(&vecColls);
	for (itVC = vecColls.begin(); itVC != vecColls.end(); itVC++)
	{
		::pDebugRenderer->addLine(origin, *itVC, cyan, 0.5f);
	}
}

void testCollisions_AABB(cGameObject* pPlayer)
{
	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	float dampSpeed = 0.9f;

	glm::vec3 origin = pPlayer->positionXYZ;
	std::vector<glm::vec3>::iterator itVC;
	unsigned long long playerAABB_ID = cAABB::getIDAABB8(pPlayer->positionXYZ);
	std::map<unsigned long long, cAABB* >::iterator itAABB = ::g_mapAABBs_World.find(playerAABB_ID);
	std::vector<glm::vec3> vecColls;
	pPlayer->getVecColliders(&vecColls);
	if (itAABB == ::g_mapAABBs_World.end()) { return; }
	else
	{
		//itAABB->second->drawAABBSelf(pDebugRenderer);
		//itAABB->second->drawAABBSelfTriangles(pDebugRenderer);

		for (itVC = vecColls.begin(); itVC != vecColls.end(); itVC++)
		{
			//::pDebugRenderer->addLine(origin, *itVC, cyan, 0.5f);
			glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
			AABBTriangle clTriangle;
			GetClosestTriangleToPoint_AABB(*itVC, itAABB->second, closestPoint, clTriangle);
			// Highlight the triangle that I'm closest to
			//pDebugRenderer->addTriangle(clTriangle.a,clTriangle.b,clTriangle.c,red);
			glm::vec3 centreOfTriangle = (clTriangle.a + clTriangle.b + clTriangle.c) / 3.0f;
			glm::vec3 normalInWorld = centreOfTriangle + (clTriangle.n * 20.0f);
			//pDebugRenderer->addLine(centreOfTriangle,normalInWorld,yellow);
			// Are we hitting the triangle?
			float distance = glm::length(*itVC - closestPoint);
			if (distance <= 2.0f)
			{
				// 1. Calculate vector from centre of sphere to closest point
				glm::vec3 vecSphereToClosestPoint = closestPoint - *itVC;
				float centreToContractDistance = glm::length(vecSphereToClosestPoint);
				float lengthPositionAdjustment = 2.0f - centreToContractDistance;

				glm::vec3 vecDirection = clTriangle.n; //glm::normalize(pPlayer->velocity);
				glm::vec3 vecPositionAdjust = (vecDirection) * lengthPositionAdjustment;

				pPlayer->positionXYZ += (vecPositionAdjust);

				pDebugRenderer->addDebugSphere(*itVC, blue, 2.0f, 1.0f);
				pDebugRenderer->addTriangle(clTriangle.a, clTriangle.b, clTriangle.c, yellow);

				// Calculate the response vector off the triangle.
				//glm::vec3 velocityVector = glm::normalize(pPlayer->velocity);
				//glm::vec3 reflectionVec = glm::reflect(velocityVector, glm::normalize(clTriangle.n));
				//reflectionVec = glm::normalize(reflectionVec);
				// Change the direction of the ball (the bounce off the triangle)
				// Get lenght of the velocity vector
				//float speed = glm::length(pPlayer->velocity);
				//pPlayer->velocity = reflectionVec * (speed * dampSpeed);
				return;
			}
		}
	}	
}

void GetClosestTriangleToPoint_AABB(glm::vec3 pointXYZ, cAABB* pAABB, glm::vec3& closestPoint, AABBTriangle& closestTriangle)
{
	float closestDistanceSoFar = FLT_MAX;
	std::set<AABBTriangle*>::iterator itABTri;
	for (itABTri = pAABB->vecABTriangles.begin(); itABTri != pAABB->vecABTriangles.end(); itABTri++)
	{
		glm::vec3 curClosetPoint = ClosestPointTriangle(pointXYZ,
			(*itABTri)->a, (*itABTri)->b, (*itABTri)->c);
		float distanceNow = glm::distance(curClosetPoint, pointXYZ);
		if (distanceNow <= closestDistanceSoFar)
		{
			closestDistanceSoFar = distanceNow;
			closestPoint = curClosetPoint;

			closestTriangle.a = (*itABTri)->a;
			closestTriangle.b = (*itABTri)->b;
			closestTriangle.c = (*itABTri)->c;
			closestTriangle.n = (*itABTri)->n;
		}
	}//for (unsigned int triIndex = 0;
	return;
}

glm::vec3 ClosestPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;
	float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);
	float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);
	if (snom <= 0.0f && tnom <= 0.0f) return a;
	float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);
	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out
	glm::vec3 n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;
	float va = glm::dot(n, glm::cross(b - p, c - p));
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

void IntegrationStep_AAB(std::map<std::string, cGameObject*> g_map_GameObjects, float deltaTime)
{
	std::map<std::string, cGameObject*>::iterator itGO;
	for (itGO = g_map_GameObjects.begin(); itGO != g_map_GameObjects.end(); itGO++)
	{
		cGameObject* pCurObj = itGO->second;
		if (pCurObj->inverseMass > 0.0f)
		{
			pCurObj->positionXYZ += pCurObj->velocity * deltaTime;
		}
	}
	pPlayerControl->updatePosition(pPlayerControl->pPlayer->positionXYZ);
	return;
}