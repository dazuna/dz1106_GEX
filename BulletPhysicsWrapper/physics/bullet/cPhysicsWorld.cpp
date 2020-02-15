#include "cPhysicsWorld.h"

namespace nPhysics
{
	cPhysicsWorld::cPhysicsWorld()
	{
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		mWorld_ = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		mWorld_->setGravity(btVector3(0, -10, 0));
	}

	cPhysicsWorld::~cPhysicsWorld()
	{
		delete mWorld_;
	}

	void cPhysicsWorld::Update(float dt)
	{
		mWorld_->stepSimulation(dt,10);
	}

	bool cPhysicsWorld::AddComponent(iPhysicsComponent* object)
	{
		// convert component to rigidBody so my library does magic :V
		switch (object->GetComponentType())
		{
		case eComponentType::ball:
			mWorld_->addRigidBody( dynamic_cast<cBallComponent*>(object)->rigidBody_ );
			return true;
			break;
		case eComponentType::plane:
			mWorld_->addRigidBody(dynamic_cast<cPlaneComponent*>(object)->rigidBody_);
			return true;
			break;
		default: break;
		}
		return false;
	}

	bool cPhysicsWorld::RemoveComponent(iPhysicsComponent* object)
	{
		switch (object->GetComponentType())
		{
		case eComponentType::ball:
			mWorld_->removeRigidBody(dynamic_cast<cBallComponent*>(object)->rigidBody_);
			return true;
			break;
		case eComponentType::plane:
			mWorld_->removeRigidBody(dynamic_cast<cPlaneComponent*>(object)->rigidBody_);
			return true;
			break;
		default: break;
		}
		return false;
	}
}