#pragma once
#include "iCommand.h"

class cFollowObject : public iCommand
{
public:
	cFollowObject(std::string name, std::string type,
		glm::vec3* followerPos, glm::vec3* targetPos,
		float TimeToFollow, glm::vec3 offsetTarget,
		float maxDistance_MaxVelocity, float minDistance_ZeroVelocity,
		float maxSpeed);
	virtual void SetGameObject(cGameObject* pGO);
	virtual void Update(double deltaTime);
	virtual bool IsDone(void);

	virtual void setName(std::string name) { this->m_Name = name; }
	virtual std::string getName(void) { return this->m_Name; }
    virtual unsigned int getUniqueID(void) { return this->m_uniqueID; }
    virtual void setType(std::string type) { this->m_Type = type; }
	virtual std::string getType(void) { return this->m_Type; }

private:
	std::string m_Name;
	std::string m_Type;
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
	cGameObject* m_pTheGO;

	float m_timer;
	float m_TimeToFollow;

	//follower position pointer
	glm::vec3* fPosition;
	//target position pointer
	glm::vec3* tPosition;
	glm::vec3 offsetFromTargetXYZ;
	float maxDistance_MaxVelocity;
	float minDistance_ZeroVelocity;
	float maxSpeed;
};
