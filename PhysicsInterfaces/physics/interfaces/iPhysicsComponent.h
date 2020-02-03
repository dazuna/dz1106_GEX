#pragma once
#include <game_math.h>
#include "eComponentType.h"

namespace nPhysics
{
	class iPhysicsComponent
	{
	public:
		virtual ~iPhysicsComponent() {}
		inline const eComponentType& GetComponentType() { return mComponentType; }

		virtual void GetTransform(glm::mat4& transformOut) = 0;

		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		virtual void ApplyForce(const glm::vec3& force) = 0;

		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		virtual void ApplyImpulse(const glm::vec3& impulse) = 0;

	protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType) {}
	private:
		eComponentType mComponentType;

		// get rid of these!
		iPhysicsComponent() = delete;
		iPhysicsComponent(const iPhysicsComponent& other) = delete;
		iPhysicsComponent& operator=(const iPhysicsComponent& other) = delete;
	};
}