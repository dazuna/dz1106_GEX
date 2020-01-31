#pragma once
#include <physics/interfaces/iPhysicsWorld.h>
#include <phys/cWorld.h>
#include "cBallComponent.h"
#include "cPlaneComponent.h"

namespace nPhysics
{
	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
		cPhysicsWorld();
		virtual ~cPhysicsWorld();
		virtual void Update(float dt);
		virtual bool AddComponent(iPhysicsComponent* object);
		virtual bool RemoveComponent(iPhysicsComponent* object);
	private:
		phys::cWorld* mWorld_;
	};
}
