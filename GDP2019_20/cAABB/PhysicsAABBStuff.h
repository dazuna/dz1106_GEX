#pragma once
#include "../cMesh.h"
#include "cAABB.h"
#include "../globalStuff.h"
#include "../DebugRenderer/cDebugRenderer.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <map>

void CalcAABBsForMeshModel(cMesh* theMesh);
void drawAABBAndTrisWherePlayerPresent(cGameObject* pPlayer);
void insertTriangleIntoAABBs(AABBTriangle* ABTri);
void insertTriangleIntoAABBs(AABBTriangle* ABTri, AABBTriangle* OGTri);
void triforceTheTriangle(AABBTriangle theTriforce, AABBTriangle* theOGTriangle);
void positionPlayerColliders(std::string objsFriendlyName);
void drawPlayerColliders(cGameObject* pPlayer);
void testCollisions_AABB(cGameObject* pPlayer);
void GetClosestTriangleToPoint_AABB(glm::vec3 pointXYZ, cAABB* pAABB, glm::vec3& closestPoint, AABBTriangle& closestTriangle);
glm::vec3 ClosestPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
void IntegrationStep_AAB(std::map<std::string, cGameObject*> g_map_GameObjects, float deltaTime);
