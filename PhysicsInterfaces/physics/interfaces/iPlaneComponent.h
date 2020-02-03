#pragma once
#include <game_math.h>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sPlaneDef
	{
		glm::vec3 Normal;
		glm::vec3 Point;
		
		sPlaneDef(glm::vec3 n, glm::vec3 p)
		: Normal(n), Point(p) {}
	};
	class iPlaneComponent : public iPhysicsComponent
	{
	public:
		virtual ~iPlaneComponent() {}

		// ApplyForce
		// The force is applied center-mass, affecting the acceleration.
		//virtual void ApplyForce(const glm::vec3& force);

		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		//virtual void ApplyImpulse(const glm::vec3& impulse);

	protected:
		iPlaneComponent() : iPhysicsComponent(eComponentType::plane) {}
	private:
		iPlaneComponent(const iPlaneComponent& other) = delete;
		iPlaneComponent& operator=(const iPlaneComponent& other) = delete;
	};
}
