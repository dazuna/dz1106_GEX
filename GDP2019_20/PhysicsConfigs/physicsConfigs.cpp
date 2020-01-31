#include "physicsConfigs.h"

nPhysics::iPhysicsFactory* g_PhysicsFactory = 0;
nPhysics::iPhysicsWorld* g_PhysicsWorld = 0;

void g_initPhysics() {
    g_PhysicsFactory = new cPhysicsFactory();
	if(g_PhysicsFactory)
	{
		printf("successfully created physics factory!\n");
	}
    g_PhysicsWorld = g_PhysicsFactory->CreateWorld();
	if (g_PhysicsWorld)
	{
		printf("successfully created physics world!\n");
	}
}

void destroyPhysics()
{
    delete g_PhysicsFactory;
    delete g_PhysicsWorld;
}