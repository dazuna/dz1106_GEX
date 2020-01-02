#pragma once
#include "../cMesh.h"
#include "cAABB.h"
#include "../globalStuff.h"
#include "../DebugRenderer/cDebugRenderer.h"
#include "../util/tools.h"
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

// FINAL PHYSICS EXAM
void AttackRun();
void generateTwoRandomPoints(glm::vec3* firstPoint, glm::vec3* secndPoint);
void drawSphereLineBetweenPoints(glm::vec3 firstPoint, glm::vec3 secondPoint);
void setXWingPosOri(glm::vec3 secondPoint);
void shootBullet(cGameObject* xwing);
void makeBulletHit(cGameObject* bullet);
void decreaseLife(std::string side);
void shouldStarDestroyerExplode(float averageDeltaTime);

void bufferSphereLine();

void testCollisions_AABB_singlePoint(cGameObject* pPlayer);
