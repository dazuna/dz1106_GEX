#pragma once
#include <physics/interfaces/interfaces.h>

namespace nPhysics
{
	class cPhysicsFactory : public iPhysicsFactory
	{
	public:
		virtual ~cPhysicsFactory() {}
		virtual iPhysicsWorld* CreateWorld();
		// components
		virtual iBallComponent* CreateBall(const sBallDef& def);
		virtual iPlaneComponent* CreatePlane(const sPlaneDef& def);
	};
}
