#ifndef _PhysicsStuff_HG_
#define _PhysicsStuff_HG_

#include <vector>
#include "cGameObject.h"

// Alias to a type "existing type" "new type name"
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

// Closest point in 3D space. XYZ 
Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);

struct Sphere
{
	glm::vec3 c;
	float r;
};
int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p);


void PhysicsUpdate(std::vector<cGameObject*>& vec_pGameObjects,
				   float deltaTime);


void HACK_BounceOffSomePlanes(cGameObject* pTheBouncingBall, bool& didItCollideWithTheGround);


#endif
