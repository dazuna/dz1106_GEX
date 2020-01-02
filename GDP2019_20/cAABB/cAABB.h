#pragma once

#include "../cMesh.h"
#include "../DebugRenderer/cDebugRenderer.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <set>
#include <map>


//map< unsigned int, cAABB* > mapAABBs_by_ID;
//
//mapAABBs_by_ID.find( theID )

class cAABB
{
public:
	cAABB() {};
	glm::vec3 minXYZ;			// 0.0, 0.0, 0.0
	float length;
	glm::vec3 getMaxXYZ(void);
	glm::vec3 getCentre(void);

	cMesh* pTheMesh;
	std::vector<sPlyTriangle*> vecTriangles;
	std::set<AABBTriangle*> vecABTriangles;

	static unsigned long long getIDAABB8(glm::vec3 testLocation)
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

		//ulX = ((unsigned long long) fabs(floor(testLocation.x / 100.0f))) * 100;
		//ulX = ((unsigned long long) fabs(floor(testLocation.x / 100))) * 100;
		//ulY = ((unsigned long long) fabs(floor(testLocation.y / 100))) * 100;
		//ulZ = ((unsigned long long) fabs(floor(testLocation.z / 100))) * 100;
		ulX = ((unsigned long long) fabs(floor(testLocation.x / 100))) * 100;
		ulY = ((unsigned long long) fabs(floor(testLocation.y / 100))) * 100;
		ulZ = ((unsigned long long) fabs(floor(testLocation.z / 100))) * 100;

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

		unsigned long long oneMillion = 1000000;

		ulX *= oneMillion * oneMillion;			// 100 * 100
		ulY *= oneMillion;

		ID = ulX + ulY + ulZ;

		return ID;
	}

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
		return true;
	};

	void drawAABBSelf(cDebugRenderer* pDebugRenderer)
	{
		glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);


		glm::vec3 maxs,
			minXmaxs, minYmaxs, minZmaxs,
			maxXmins, maxYmins, maxZmins;

		maxs = minXYZ+length;

		minXmaxs = glm::vec3(minXYZ.x, maxs.y, maxs.z);
		minYmaxs = glm::vec3(maxs.x, minXYZ.y, maxs.z);
		minZmaxs = glm::vec3(maxs.x, maxs.y, minXYZ.z);

		maxXmins = glm::vec3(maxs.x, minXYZ.y, minXYZ.z);
		maxYmins = glm::vec3(minXYZ.x, maxs.y, minXYZ.z);
		maxZmins = glm::vec3(minXYZ.x, minXYZ.y, maxs.z);

		// edges that touch mins
		pDebugRenderer->addLine(minXYZ, maxXmins, blue, 0.01f);
		pDebugRenderer->addLine(minXYZ, maxYmins, white, 0.01f);
		pDebugRenderer->addLine(minXYZ, maxZmins, white, 0.01f);

		// edges that touch maxs
		pDebugRenderer->addLine(maxs, minXmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxs, minYmaxs, red, 0.01f);
		pDebugRenderer->addLine(maxs, minZmaxs, white, 0.01f);

		// other edges
		pDebugRenderer->addLine(maxYmins, minXmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxYmins, minZmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxZmins, minXmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxZmins, minYmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxXmins, minYmaxs, white, 0.01f);
		pDebugRenderer->addLine(maxXmins, minZmaxs, white, 0.01f);
	}

	void drawAABBSelfTriangles(cDebugRenderer* pDebugRenderer)
	{
		glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);

		std::set<AABBTriangle*>::iterator itABTri;
		for (itABTri = this->vecABTriangles.begin(); itABTri != this->vecABTriangles.end(); itABTri++)
		{
			AABBTriangle ABTri = **itABTri;
			pDebugRenderer->addLine(ABTri.a, ABTri.b, cyan, 0.1f);
			pDebugRenderer->addLine(ABTri.b, ABTri.c, cyan, 0.1f);
			pDebugRenderer->addLine(ABTri.c, ABTri.a, cyan, 0.1f);
		}
	}
};
