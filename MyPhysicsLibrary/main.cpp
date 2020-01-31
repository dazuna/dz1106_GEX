#include "phys/phys.h"
#include "game_math.h"
#include <iostream>

int main()
{
	printf("starting library trials\n");
	printf("adding spheres & planes\n");

	phys::cWorld* theWorld = new phys::cWorld();
	// the rigidBodies to add
	phys::iShape* sphereShape = new phys::cSphere(2.f);
	phys::iShape* planeShape = new phys::cPlane(glm::vec3(0, 1, 0), 0.f);
	phys::sRigidBodyDef sphere1Def = phys::sRigidBodyDef(1.f, glm::vec3(0, 5, 0), glm::vec3(0.f));
	phys::sRigidBodyDef sphere2Def = phys::sRigidBodyDef(1.f, glm::vec3(20, 5, 20), glm::vec3(0.f));
	phys::sRigidBodyDef planeDef = phys::sRigidBodyDef(0.f, glm::vec3(0), glm::vec3(0));

	phys::cRigidBody* sphere1 = new phys::cRigidBody(sphere1Def, sphereShape);
	phys::cRigidBody* sphere2 = new phys::cRigidBody(sphere2Def, sphereShape);
	phys::cRigidBody* plane = new phys::cRigidBody(planeDef, planeShape);

	theWorld->AddRigidBody(sphere1);
	theWorld->AddRigidBody(sphere2);
	theWorld->AddRigidBody(plane);

	int i = 0;
	while ( i < 50)
	{
		theWorld->Update(0.3f);
		i++;
	}
	
	system("pause");
	return 0;
}
