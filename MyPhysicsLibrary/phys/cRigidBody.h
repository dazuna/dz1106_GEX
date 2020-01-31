#pragma once
#include <game_math.h>  // single include header for all glm math stuffs
#include "iShape.h"     // iShape base class for all shapes

namespace phys
{
	// sRigidBodyDef
	// Contains all non-shape related information
	// necessary to create a cRigidBody instance.
	struct sRigidBodyDef
	{
		//Create a default constructor
		// Constructor:
		sRigidBodyDef()
		: Mass(0.f), Position(glm::vec3(0)), Velocity(glm::vec3(0)){}
		
		sRigidBodyDef(float mass,glm::vec3 pos,glm::vec3 vel)
		: Mass(mass), Position(pos), Velocity(vel) {}

		// Mass
		// Must be set to a non-negative value.
		// 0.f will result in a static rigid body.
		// A positive value will result in a dynamic rigid body.
		float Mass;
		// Position // The initial position of the rigid body.
		glm::vec3 Position;
		// Velocity // The initial linear velocity of the rigid body.
		glm::vec3 Velocity;
	};

	// cRigidBody
	//
	// Contains all values required for a rigid body simulation.
	// 
	// Shapes may be shared between rigid bodies.
	// Does not own the iShape* used to create it.
	// Will not delete the iShape* it contains when it is deleted.
	class cRigidBody
	{
		// cWorld will be operating on cRigidBody values quite a bit
		// We will trust it to do everything correctly.
		friend class cWorld;
	public:
		// Constructor
		// All internal values are initialized according to the sRigidBodyDef
		// A valid iShape* is expected.
		cRigidBody(const sRigidBodyDef& def, iShape* shape);

		// Destructor
		// Will not delete the contained iShape.
		~cRigidBody();

		// GetTransform
		// Retrieve a rendering-friendly form of position + rotation
		void GetTransform(glm::mat4& transformOut);

		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		void ApplyForce(const glm::vec3& force);

		// TODO:
		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		void ApplyImpulse(const glm::vec3& impulse);

		// IsStatic
		// A static rigid body was created with a mass of 0.f.
		// A static rigid body does react to other rigid bodies in the world.
		inline bool IsStatic() { return mMass == 0.f; }

		// GetShape
		// Get this rigid body's shape.
		// Each rigid body must be created with a shape.
		inline iShape* GetShape() const { return mShape; }

		// GetShapeType
		// Get the type of shape of this rigid body's shape.
		// Equivalent to GetShape()->GetShapeType()
		inline eShapeType GetShapeType() { return mShape->GetShapeType(); }

	private:
		// My shape, expected to be valid.
		// cRigidBody will not delete mShape in its destructor.
		iShape* mShape;

		// During a timestep, the previous position is stored for collision related purposes.
		glm::vec3 mPreviousPosition;
		// Current linear position.
		glm::vec3 mPosition;
		// Current linear velocity.
		glm::vec3 mVelocity;
		// Current linear acceleration.
		glm::vec3 mAcceleration;
		// Mass
		// Expected to be non-negative.
		// A value of 0 infers a static rigid body.
		float mMass;
		// Inverse mass
		// Stored for convenience.
		// If mMass is 0, mInvMass will also be 0.
		float mInvMass;

		// Constructors not to be used.
		cRigidBody() = delete;
		cRigidBody(const cRigidBody& other) = delete;
		cRigidBody& operator=(const cRigidBody& other) = delete;
	};
}