#include "cBallComponent.h"
#include "nConvert.h"

namespace nPhysics
{
	cBallComponent::cBallComponent(const sBallDef& def)
	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		//btCollisionShape* shape = new btSphereShape(def.Radius);
		sphereShape_ = new btSphereShape(def.Radius);

		/// Create Dynamic Objects
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(nConvert::toBullet(def.Position));

		btScalar mass(def.Mass);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			sphereShape_->calculateLocalInertia(mass, localInertia);
		}

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, sphereShape_, localInertia);
		rbInfo.m_restitution = 0.5f;
		rigidBody_ = new btRigidBody(rbInfo);
		rigidBody_->setUserPointer(this);
	}

	cBallComponent::~cBallComponent()
	{
		delete rigidBody_->getMotionState();
		delete rigidBody_;
		rigidBody_ = 0;
	}

	void cBallComponent::GetTransform(glm::mat4& transformOut)
	{
		btTransform transform;
		this->rigidBody_->getMotionState()->getWorldTransform(transform);
		nConvert::toGLM(transform,transformOut);
	}

	void cBallComponent::ApplyForce(const glm::vec3& force)
	{
		this->rigidBody_->activate(true);
		this->rigidBody_->applyCentralForce(nConvert::toBullet(force));
	}

	void cBallComponent::ApplyImpulse(const glm::vec3& impulse)
	{
		this->rigidBody_->applyImpulse(nConvert::toBullet(impulse),btVector3(0.f,0.f,0.f));
	}
}
