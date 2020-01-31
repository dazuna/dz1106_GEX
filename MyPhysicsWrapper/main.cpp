#include "phys/phys.h"
#include "game_math.h"
#include "cPhysicsWorld.h"
#include "cBallComponent.h"
#include "cPlaneComponent.h"
#include <iostream>

using namespace nPhysics;

int main()
{
	
	
	printf("starting library trials\n");
	printf("adding spheres & planes\n");

	auto theWorld = new cPhysicsWorld();

	sBallDef ballDef(1.f, 2.f, glm::vec3(0, 5, 0), glm::vec3(0,-5,0));
	sPlaneDef planeDef(glm::vec3(0, 1, 0), glm::vec3(0));

	cBallComponent cBall(ballDef);
	cPlaneComponent cPlane(planeDef);

	theWorld->AddComponent(&cBall);
	theWorld->AddComponent(&cPlane);

	int i = 0;
	while (i < 50)
	{
		theWorld->Update(0.03f);
		i++;
	}

	system("pause");
	return 0;
}
