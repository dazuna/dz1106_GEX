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

		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		virtual void ApplyForce(const glm::vec3& force);

		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		virtual void ApplyImpulse(const glm::vec3& impulse);
	private:
		phys::cRigidBody* rigidBody_;
		phys::iShape* sphereShape_;
	};
}
