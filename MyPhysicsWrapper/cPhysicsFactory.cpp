#include "cPhysicsFactory.h"
#include "wrapper.h"

namespace nPhysics
{
	iPhysicsWorld* cPhysicsFactory::CreateWorld()
	{
		auto theWorld = new cPhysicsWorld();
		return theWorld;
	}
	iBallComponent* cPhysicsFactory::CreateBall(const sBallDef& def)
	{
		cBallComponent* cBall = new cBallComponent(def);
		return cBall;
	}
	iPlaneComponent* cPhysicsFactory::CreatePlane(const sPlaneDef& def)
	{
		cPlaneComponent* cPlane = new cPlaneComponent(def);
		return cPlane;
	}
}
