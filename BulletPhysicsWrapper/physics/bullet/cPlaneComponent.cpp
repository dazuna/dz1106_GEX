#include "cPlaneComponent.h"
#include "nConvert.h"

namespace nPhysics
{
	cPlaneComponent::cPlaneComponent(const sPlaneDef& def)
	{
		planeShape_ = 
			new btStaticPlaneShape(nConvert::toBullet(def.Normal),
				btDot(nConvert::toBullet(def.Normal),nConvert::toBullet(def.Point)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		//groundTransform.setOrigin(nConvert::toBullet(def.Point));

		btScalar mass(0.);
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, planeShape_, localInertia);
		rbInfo.m_restitution = 0.5f;
		rigidBody_ = new btRigidBody(rbInfo);
		rigidBody_->setUserPointer(this);
	}

	cPlaneComponent::~cPlaneComponent()
	{
		delete rigidBody_->getMotionState();
		delete rigidBody_;
		rigidBody_ = 0;
	}
	
	void cPlaneComponent::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		this->rigidBody_->getMotionState()->getWorldTransform(transform);
		nConvert::toGLM(transform,transformOut);
	}

	void cPlaneComponent::ApplyForce(const glm::vec3& force)
	{
		this->rigidBody_->applyCentralForce(nConvert::toBullet(force));
	}

	void cPlaneComponent::ApplyImpulse(const glm::vec3& impulse)
	{
		this->rigidBody_->applyImpulse(nConvert::toBullet(impulse),btVector3(0.f,0.f,0.f));
	}
}