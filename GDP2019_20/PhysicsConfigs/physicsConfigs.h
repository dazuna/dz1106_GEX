/* Macro to interchange the libraries */
//#define MY_PHYSICS 
#define BULLET_PHYSICS

#include <physics/interfaces/interfaces.h>

#ifdef MY_PHYSICS
#include <cPhysicsFactory.h> // inside my wrapper project
#pragma  comment (lib, "MyPhysicsLibrary.lib")
#pragma comment (lib, "MyPhysicsWrapper.lib")
typedef nPhysics::cPhysicsFactory cPhysicsFactory;
#endif

#ifdef BULLET_PHYSICS
#include <cPhysicsFactory.h> // inside my wrapper project
#pragma comment (lib, "BulletPhysicsWrapper.lib")
typedef nPhysics::cPhysicsFactory cPhysicsFactory;
#endif

// Bullet includes

extern nPhysics::iPhysicsFactory* g_PhysicsFactory;
extern nPhysics::iPhysicsWorld* g_PhysicsWorld;

void g_initPhysics();
void destroyPhysics();
