#include "cPlaneComponent.h"
namespace nPhysics
{
	using namespace phys;
	cPlaneComponent::cPlaneComponent(const sPlaneDef& def)
	{
		sRigidBodyDef def2 = sRigidBodyDef(0.f, def.Point, glm::vec3(0.f));
		float constant = glm::dot(def.Point, def.Normal);
		this->planeShape_ = new cPlane(def.Normal,constant);
		this->rigidBody_ = new phys::cRigidBody(def2, planeShape_);
	}

	cPlaneComponent::~cPlaneComponent()
	{
		delete this->rigidBody_;
		delete this->planeShape_;
	}
	
	void cPlaneComponent::GetTransform(glm::mat4& transformOut)
	{
		this->rigidBody_->GetTransform(transformOut);
	}

	void cPlaneComponent::ApplyForce(const glm::vec3& force)
	{
		this->rigidBody_->ApplyForce(force);
	}

	void cPlaneComponent::ApplyImpulse(const glm::vec3& impulse)
	{
		this->rigidBody_->ApplyImpulse(impulse);
	}
}