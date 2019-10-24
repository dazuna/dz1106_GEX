#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

class cParticle
{
private:
    /* data */
public:
    cParticle(/* args */);
    glm::vec3 location;
    glm::vec3 velocity;
    glm::vec4 colourRGBA;
    float lifeTime;
    // orientation
    // scale
    // acceleration
};

class cParticleEmitter
{
private:
    /* data */
public:
    cParticleEmitter(/* args */);
    glm::vec3 location;
    glm::vec3 acceleration;
    void Init(glm::vec3 minVelocity, glm::vec3 maxVelocity,
            glm::vec3 minDeltaPosition, glm::vec3 maxDeltaPosition,
            float minLifeTime, float maxLifeTime,
            int minParticlesPerFrame,int maxParticlesPerFrame);
    void Step(float deltaTime);
    void getParticles(std::vector<cParticle*> &m_vec_pParticles);
private:
	static const unsigned int DEFAULT_NUM_PARTICLES=1000;
    std::vector<cParticle*> m_vec_pParticles;

	template <class T>
	T randInRange(T min, T max)
	{
		double value =
			min + static_cast <double> (rand())
			/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
		return static_cast<T>(value);
	};

	void m_CreateNewParticles(glm::vec3 minVelocity, glm::vec3 maxVelocity,
		glm::vec3 minDeltaPosition, glm::vec3 maxDeltaPosition,
		float minLifeTime, float maxLifeTime);

    glm::vec3 m_minVelocity;
    glm::vec3 m_maxVelocity;
    glm::vec3 m_minDeltaPosition;
    glm::vec3 m_maxDeltaPosition;
    float m_minLifeTime;
    float m_maxLifeTime;
    int m_minParticlesPerFrame;
    int m_maxParticlesPerFrame;
};

