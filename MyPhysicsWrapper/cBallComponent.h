#pragma once
#include <physics/interfaces/interfaces.h>
#include <phys/phys.h>
#include "cPhysicsWorld.h"

namespace nPhysics
{
	class cBallComponent : public iBallComponent
	{
		friend class cPhysicsWorld;
	public:
		cBallComponent(const sBallDef& def);
		virtual ~cBallComponent();
		virtual void GetTransform(glm::mat4& transformOut);
	private:
		phys::cRigidBody* rigidBody_;
		phys::iShape* sphereShape_;
	};
}
