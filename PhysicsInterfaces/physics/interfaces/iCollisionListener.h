#pragma once

namespace nPhysics
{
	class iCollisionListener
	{
	public:
		virtual ~iCollisionListener() = 0;
		// virtual void Collide(iPhysicsComponent* compA, iPhysicsComponent* compB) = 0;
	};
}