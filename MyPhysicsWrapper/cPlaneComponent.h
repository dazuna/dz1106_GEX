#pragma once
#include <physics/interfaces/interfaces.h>
#include <phys/phys.h>
#include "cPhysicsWorld.h"

namespace nPhysics
{
	class cPlaneComponent : public iPlaneComponent
	{
		friend class cPhysicsWorld;
	public:
		cPlaneComponent(const sPlaneDef& def);
		virtual ~cPlaneComponent();
		virtual void GetTransform(glm::mat4& transformOut);
	private:
		phys::cRigidBody* rigidBody_;
		phys::iShape* planeShape_;
	};
}
