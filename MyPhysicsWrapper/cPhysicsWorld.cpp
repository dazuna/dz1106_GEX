#include "cPhysicsWorld.h"

namespace nPhysics
{
	cPhysicsWorld::cPhysicsWorld()
	{
		mWorld_ = new phys::cWorld();
	}

	cPhysicsWorld::~cPhysicsWorld()
	{
		delete mWorld_;
	}

	void cPhysicsWorld::Update(float dt)
	{
		mWorld_->Update(dt);
	}

	bool cPhysicsWorld::AddComponent(iPhysicsComponent* object)
	{
		// convert component to rigidBody so my library does magic :V
		switch (object->GetComponentType())
		{
		case eComponentType::ball:
			return mWorld_->AddRigidBody(dynamic_cast<cBallComponent*>(object)->rigidBody_);
			break;
		case eComponentType::plane:
			return mWorld_->AddRigidBody(dynamic_cast<cPlaneComponent*>(object)->rigidBody_);
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
			return mWorld_->RemoveRigidBody(dynamic_cast<cBallComponent*>(object)->rigidBody_);
			break;
		case eComponentType::plane:
			return mWorld_->RemoveRigidBody(dynamic_cast<cPlaneComponent*>(object)->rigidBody_);
			break;
		default: break;
		}
		return false;
	}
}