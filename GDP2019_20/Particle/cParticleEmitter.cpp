#include "cParticleEmitter.h"
#include <algorithm>

cParticle::cParticle()
{
    this->location = glm::vec3(0.0f,0.0f,0.0f);
    this->velocity = glm::vec3(0.0f,0.0f,0.0f);
    this->colourRGBA = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    this->lifeTime = 0.0f;

    return;
}

cParticleEmitter::cParticleEmitter()
{
	this->location = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	return;
}

void cParticleEmitter::Init(glm::vec3 minVelocity, glm::vec3 maxVelocity, glm::vec3 minDeltaPosition, glm::vec3 maxDeltaPosition, float minLifeTime, float maxLifeTime, int minParticlesPerFrame, int maxParticlesPerFrame)
{
	//this->m_vec_pParticles.clear();
	this->m_vec_pParticles.reserve(cParticleEmitter::DEFAULT_NUM_PARTICLES);
	for (int count = 0; count != cParticleEmitter::DEFAULT_NUM_PARTICLES; count++)
	{
		cParticle* pNewParticle = new cParticle();
		pNewParticle->lifeTime = 0.0f;
		this->m_vec_pParticles.push_back(pNewParticle);
	}

    this->m_minVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_maxVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_minDeltaPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_maxDeltaPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_minLifeTime = 0.0f;
    this->m_maxLifeTime = 0.0f;
    this->m_minParticlesPerFrame = 0.0f;
    this->m_maxParticlesPerFrame = 0.0f;

    return;
}

void cParticleEmitter::Step(float deltaTime)
{
	//loop thru particles 
	// - Decrease the life by deltaTime
	// - if still 'alive' 
	//   - move the particle
	for (std::vector<cParticle*>::iterator itPart = m_vec_pParticles.begin();
		itPart != m_vec_pParticles.end();
		itPart++)
	{
		cParticle* pCurParticle = *itPart;
		pCurParticle->lifeTime -= deltaTime;
		// is it dead?
		if (pCurParticle->lifeTime > 0.0f)
		{
			//updt velocity from accel
			pCurParticle->velocity += (this->acceleration * deltaTime);
			//updt pos from velocity
			pCurParticle->location += (pCurParticle->velocity * deltaTime);

		}		// </ if (pCurParticle->lifeTime > 0.0f)
	}
	return;
}

void cParticleEmitter::getParticles(std::vector<cParticle*>& m_vec_pParticles)
{
}

void cParticleEmitter::m_CreateNewParticles(
	glm::vec3 minVelocity, glm::vec3 maxVelocity,
	glm::vec3 minDeltaPosition, glm::vec3 maxDeltaPosition,
	float minLifeTime, float maxLifeTime)
{
	//find the 1st particle that has life larger than 0
	for (std::vector<cParticle*>::iterator itPart = m_vec_pParticles.begin();
		itPart != m_vec_pParticles.end();
		itPart++)
	{
		cParticle* pCurParticle = *itPart;
		if (pCurParticle->lifeTime <= 0.0f)
		{
			pCurParticle->lifeTime = randInRange<float>(minLifeTime, maxLifeTime);

			pCurParticle->velocity.x = randInRange<float>(minVelocity.x, maxVelocity.x);
			pCurParticle->velocity.y = randInRange<float>(minVelocity.y, maxVelocity.y);
			pCurParticle->velocity.z = randInRange<float>(minVelocity.z, maxVelocity.z);

		}		// </ if (pCurParticle->lifeTime <= 0.0f)
	}
    int numParticleToCreate = randInRange<int>(
        this->m_minParticlesPerFrame,this->m_maxParticlesPerFrame);
    
}
