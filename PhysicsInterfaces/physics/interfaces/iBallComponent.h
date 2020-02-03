#pragma once
#include <game_math.h>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sBallDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::vec3 Velocity;

		sBallDef(float m, float r, glm::vec3 p, glm::vec3 v)
		:Mass(m),Radius(r),Position(p),Velocity(v) {}
	};

	class iBallComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBallComponent() {}
		
		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		//virtual void ApplyForce(const glm::vec3& force);

		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		//virtual void ApplyImpulse(const glm::vec3& impulse);
		
	protected:
		iBallComponent() : iPhysicsComponent(eComponentType::ball) {}
	private:
		iBallComponent(const iBallComponent& other) = delete;
		iBallComponent& operator=(const iBallComponent& other) = delete;
	};
}