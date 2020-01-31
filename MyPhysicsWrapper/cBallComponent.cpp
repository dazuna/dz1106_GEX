#include "cBallComponent.h"
namespace nPhysics
{
	using namespace phys;
	cBallComponent::cBallComponent(const sBallDef& def)
	{
		sRigidBodyDef def2 = sRigidBodyDef(def.Mass, def.Position, def.Velocity);
		this->sphereShape_ = new cSphere(def.Radius);
		this->rigidBody_ = new phys::cRigidBody(def2, sphereShape_);
	}

	cBallComponent::~cBallComponent()
	{
		delete this->rigidBody_;
		delete this->sphereShape_;
	}

	void cBallComponent::GetTransform(glm::mat4& transformOut)
	{
		this->rigidBody_->GetTransform(transformOut);
	}
	
}
