#include "cWorld.h"    // My header
#include "nCollide.h"  // collision detection functions from
#include <iostream>

// REAL-TIME COLLISION DETECTION, ERICSON

namespace phys
{
	cWorld::cWorld()
	{
		// Initialize all internal variables to reasonable values
		this->mDt = 1.f / 60.f;
		this->mIntegrator;
		// things should fall, probably
		this->mGravity=glm::vec3(0, -1, 0);
		this->mBodies = {};
	}

	// all variables heap based so, no custom behaviour needed
	cWorld::~cWorld() = default;
	
	void cWorld::GetGravity(glm::vec3& gravityOut)
	{
		gravityOut = mGravity;
	}

	void cWorld::SetGravity(const glm::vec3& gravityIn)
	{
		mGravity = gravityIn;
	}

	void cWorld::Update(float dt)
	{
		// 1) If we have no bodies, there's nothing to do... return.
		if(mBodies.empty()) { return; }
		// 2) Integrate each body.
		for (const auto & body : mBodies)
		{
			IntegrateRigidBody(body, dt);
			auto type = body->GetShapeType();
			if (type == eShapeType::sphere)
				std::cout << "pos: " << glm::to_string(body->mPosition) << "   \tvel: " << glm::to_string(body->mVelocity) << std::endl;
			else
				std::cout << std::endl;
		}
		// 3) Perform collision handling on each unique pair of bodies.
		for (size_t idxA = 0; idxA < mBodies.size() - 1; idxA++)
		{
			for (size_t idxB = idxA + 1; idxB < mBodies.size(); idxB++)
			{
				Collide(mBodies[idxA], mBodies[idxB]);
			}
		}
		// 4) Clear the acceleration of each rigid body.
		for (const auto& body : mBodies)
		{
			body->mAcceleration = glm::vec3(0.f);
		}
	}

	bool cWorld::AddRigidBody(cRigidBody* rigidBody)
	{
		// 1) Null check
		if (!rigidBody) return false;
		// 2) Check if we currently have this rigid body.
		const auto itbody = std::find(mBodies.begin(), mBodies.end(), rigidBody);
		//    If yes: Return false to indicate nothing was added.
		if (itbody != mBodies.end()) return false;
		//    If no:  Add it, then return true to indicate it was added.
		mBodies.push_back(rigidBody);
		return true;
	}

	bool cWorld::RemoveRigidBody(cRigidBody* rigidBody)
	{
		// 1) Null check
		if (!rigidBody) return false;
		// 2) Check if we currently have this rigid body.
		const auto itbody = std::find(mBodies.begin(), mBodies.end(), rigidBody);
		//    If no:  Return false to indicate nothing was removed.
		if (itbody == mBodies.end()) return false;
		//    If yes: Remove it, then return true to indicate it was removed.
		mBodies.erase(itbody);
		return true;
	}

	void cWorld::IntegrateRigidBody(cRigidBody* body, float dt)
	{
		// 1) Static bodies are not to be integrated!
		if (body->IsStatic()) { return; }
		// 2) Update the body's previous position.
		body->mPreviousPosition = body->mPosition;
		// 3) Do some integrating!
		mIntegrator.RK4(
			body->mPosition,
			body->mVelocity,
			body->mAcceleration,
			this->mGravity, dt);
		// 4) Apply some linear damping
		body->mVelocity *= glm::pow(0.95f, dt);
	}
	
	bool cWorld::Collide(cRigidBody* bodyA, cRigidBody* bodyB)
	{
		// 1) Based on shape type, determine which specific collision handling method to use.		
		// 2) Cast up the shapes, call the methods, return the result.
		eShapeType shapeTypeA = bodyA->GetShapeType();
		eShapeType shapeTypeB = bodyB->GetShapeType();
		
		if (shapeTypeA == eShapeType::plane)
		{
			if (shapeTypeB == eShapeType::plane) { return false; }
			if (shapeTypeB == eShapeType::sphere)
			{
				return CollideSpherePlane(
					bodyB, dynamic_cast<cSphere*>(bodyB->GetShape()),
					bodyA, dynamic_cast<cPlane*>(bodyA->GetShape()));
			}
		}
		if (shapeTypeA == eShapeType::sphere)
		{
			if (shapeTypeB == eShapeType::plane)
			{
				return CollideSpherePlane(
					bodyA, dynamic_cast<cSphere*>(bodyA->GetShape()),
					bodyB, dynamic_cast<cPlane*>(bodyB->GetShape()));
			}
			if (shapeTypeB == eShapeType::sphere)
			{
				return CollideSphereSphere(
					bodyB, dynamic_cast<cSphere*>(bodyB->GetShape()),
					bodyA, dynamic_cast<cSphere*>(bodyA->GetShape()));
			}
		}
		return false;
	}

	bool cWorld::CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape,
		cRigidBody* planeBody, cPlane* planeShape)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_plane(...inputs...outputs...)
		// to determine if:
		// case A: The sphere did not collide during the timestep.
		// case B: The sphere was already colliding at the beginning of the timestep.
		// case C: The sphere collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for this sphere after applying an
		//         impulse that should separate it from the plane.
		// 
		// 1) From our textbook, use closest_point_on_plane(..inputs..) to determine the 
		//    penetration-depth of the sphere at the beginning of the timestep.
		//    (This penetration-depth is the distance the sphere must travel during
		//    the timestep in order to escape the plane.)
		// 2) Use the sphere's center and the closest point on the plane to define
		//    the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply the impulse vector to sphere velocity.
		// 5) Reset the sphere position.
		// 6) Re-do the integration for the timestep.
		// 7) Return true to indicate a collision has happened.
		// 
		// 
		// case C: Rewind to the point of impact, reflect, then replay.
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the sphere position to the point of impact.
		// 2) Reflect the sphere's velocity about the plane's normal vector.
		// 3) Apply some energy loss (to the velocity) in the direction
		//    of the plane's normal vector.
		// 4) Re-integrate the sphere with its new velocity over the remaining
		//    portion of the timestep.
		// 5) Return true to indicate a collision has happened.

		auto c = sphereBody->mPreviousPosition;
		auto r = sphereShape->GetRadius();
		auto v = sphereBody->mPosition- sphereBody->mPreviousPosition;
		auto n = planeShape->GetNormal();
		auto d = planeShape->GetConstant();
		float t(0.f);
		glm::vec3 q;

		int result = nCollide::intersect_moving_sphere_plane(c, r, v, n, d, t, q);
		if (result == 0)
		{
			//no collision
			return false;
		}
		if (result == -1)
		{
			// already colliding

			glm::vec3 pointOnPlane = nCollide::closest_point_on_plane(sphereBody->mPreviousPosition,
				planeShape->GetNormal(), planeShape->GetConstant());
			// impulse for sphere to escape the plane
			float distance = glm::distance(sphereBody->mPreviousPosition, pointOnPlane);
			float targetDistance = r;
			glm::vec3 impulse = n * (targetDistance - distance) / mDt;
			// reset position
			sphereBody->mPosition = sphereBody->mPreviousPosition;
			// apply impulse
			sphereBody->mVelocity += impulse;
			// redo timeStep
			IntegrateRigidBody(sphereBody, mDt);

			return true;
		}
		// has collided
		
		//reflect
		sphereBody->mVelocity = glm::reflect(sphereBody->mVelocity, planeShape->GetNormal());
		// Lose energy
		glm::vec3 nComponent = glm::proj(sphereBody->mVelocity, planeShape->GetNormal());
		sphereBody->mVelocity -= nComponent * 0.2f;
		// back to the future
		sphereBody->mPosition = (c + v * t);
		// integrate
		IntegrateRigidBody(sphereBody, mDt * (1.f - t));
		// absolute success
		return false;
	}

	bool cWorld::CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA, 
		cRigidBody* bodyB, cSphere* shapeB)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_sphere(...inputs...outputs...)
		// to determine if:
		// case A: The spheres don't collide during the timestep.
		// case B: The spheres were already colliding at the beginning of the timestep.
		// case C: The spheres collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for these spheres after
		//         applying an impulse that should separate them.
		// 
		// 1) Determine the penetration-depth of the spheres at the beginning of the timestep.
		//    (This penetration-depth is the distance the spheres must travel during
		//    the timestep in order to separate.)
		// 2) Use the sphere's centers to define the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply a portion of the impulse vector to sphereA's velocity.
		// 5) Apply a portion of the impulse vector to sphereB's velocity.
		//    (Be sure to apply the impulse in opposing directions.)
		// 6) Reset the spheres' positions.
		// 7) Re-do the integration for the timestep.
		// 8) Return true to indicate a collision has happened.
		// 
		// 
		// case C: 
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the spheres positions to the point of impact.
		// 2) Use the inelastic collision response equations from
		//    Wikepedia to set they're velocities post-impact.
		// 3) Re-integrate the spheres with their new velocities
		//    over the remaining portion of the timestep.
		// 4) Return true to indicate a collision has happened.
		//

		glm::vec3 cA = bodyA->mPreviousPosition;
		glm::vec3 cB = bodyB->mPreviousPosition;
		glm::vec3 vA = bodyA->mPosition - bodyA->mPreviousPosition;
		glm::vec3 vB = bodyB->mPosition - bodyB->mPreviousPosition;
		float rA = shapeA->GetRadius();
		float rB = shapeB->GetRadius();
		float t(0.f);

		int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
		if (result == 0) { /*no collision!*/ return false; }

		//get the masses
		float ma = bodyA->mMass;
		float mb = bodyB->mMass;
		float mt = ma + mb;

		if (result == -1)
		{
			/*already colliding*/
			float initialDistance = glm::distance(bodyA->mPreviousPosition, bodyB->mPreviousPosition);
			float targetDistance = rA + rB;

			glm::vec3 impulseToA = glm::normalize(bodyA->mPreviousPosition - bodyB->mPreviousPosition);
			impulseToA *= (targetDistance - initialDistance);

			// back to previous
			bodyA->mPosition = bodyA->mPreviousPosition;
			bodyB->mPosition = bodyB->mPreviousPosition;
			// apply impulse, transf energy between them
			bodyA->mVelocity += impulseToA * (mb / mt);
			bodyB->mVelocity -= impulseToA * (mb / mt);
			// integrate
			IntegrateRigidBody(bodyA, mDt);
			IntegrateRigidBody(bodyB, mDt);
			return true;
		}
		// collision

		// back to point of collision
		bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
		bodyB->mPosition = bodyB->mPreviousPosition + vB * t;
		//bounce            //      wikipedia (in)elastic collision
		vA = bodyA->mVelocity;
		vB = bodyB->mVelocity;

		float c = 0.2f;
		bodyA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		bodyB->mVelocity = (c * ma * (vA - vB) + ma * vA + mb * vB) / mt;

		IntegrateRigidBody(bodyA, mDt * (1.f - t));
		IntegrateRigidBody(bodyB, mDt * (1.f - t));

		return false;
	}
}
