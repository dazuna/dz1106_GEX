#include "cRigidBody.h"  // My header

namespace phys
{
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape)
	{
		// Initialize all internal values according 'def' and 'shape'
		this->mShape = shape;
		// Same pos since it's initial position... (prev & current)
		this->mPreviousPosition = def.Position;
		this->mPosition = def.Position;
		this->mVelocity = def.Velocity;
		this->mAcceleration = glm::vec3(0);
		this->mMass = def.Mass;
		this->mInvMass = 1.f / def.Mass;
	}

	// heap based variables
	// (except iShape, which should not be deleted)
	cRigidBody::~cRigidBody() = default;

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		// Populate transformOut with correct values.
		transformOut = glm::translate(glm::mat4(1), this->mPosition);
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		// Apply the force to this rigid body's acceleration
		// Remember... F=ma  ...so an applied acceleration will be f/m
		this->mAcceleration += force * this->mInvMass;
	}
	
	void cRigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		// Apply the impulse to this rigid body's velocity
		this->mVelocity += impulse;
	}
}
