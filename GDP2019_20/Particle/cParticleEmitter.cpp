#include "cParticleEmitter.h"
#include "../cGameObject.h"
#include <glad/glad.h>
#include "../util/tools.h"

cParticleEmitter* cParticleEmitter::PEmtr = new cParticleEmitter();

cParticle::cParticle()
{
	// Note: we aren't really using these default values...
	this->location = glm::vec3(0.0f,0.0f,0.0f);
	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->lifeTime = 0.0f;
	this->colourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->qOrientation = glm::quat( glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->scaleChangePercentPerSecond = 0.0f;
	this->qRotationalChangePerSecond = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->bImposterFacesCamera = false;
	this->textureName = "";
	// Default is no change over time
	this->alphaChangePerSecond = 0.0f;
	return;
}

cParticleEmitter::sParticleCreationSettings::sParticleCreationSettings()
{
	this->minLifeSeconds = this->maxLifeInSeconds = 0.0f;
	this->numParticlesPerSecond = 0;
	this->minVelocity = this->maxVelocity = glm::vec3(0.0f);
	this->particleCreationVolume = AABB;
	this->minDeltaAABBPosition = this->maxDeltaAABBPosition = glm::vec3(0.0f);
	this->minDeltaRadiusPosition = this->maxDeltaRadiusPosition = 0.0f;
	this->minStartingScale = this->maxStartingScale = 1.0f;
	this->minScaleChangePercentPerSecond = this->maxScaleChangePercentPerSecond = 0.0f;
	// Rotational change is 0 degrees on all axes
	this->minRotationalChangePerSecond = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->maxRotationalChangePerSecond = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->isImposterFaceCamera = false;
	this->bFadeOutOverLifetime = false;
	return;
}

void cParticleEmitter::setSPCSDefaultValues(sParticleCreationSettings& sPCS)
{
	sPCS.minLifeSeconds = float(0.1);
	sPCS.maxLifeInSeconds = float(5.5);
	sPCS.particleCreationVolume = cParticleEmitter::sParticleCreationSettings::SPHERE;
	sPCS.minVelocity = glm::vec3(-20);
	sPCS.maxVelocity = glm::vec3(20);
	sPCS.minStartingScale = 0.01f;
	sPCS.maxStartingScale = 0.1f;
	sPCS.minScaleChangePercentPerSecond = 0.1f;	// 10%
	sPCS.maxScaleChangePercentPerSecond = 0.2f;	// 100%
	sPCS.numParticlesPerSecond = float(500);
	sPCS.isImposterFaceCamera = true;
	sPCS.bFadeOutOverLifetime = false;
}

cParticleEmitter::cParticleEmitter()
{
	// NOTE: I'm NOT setting up the particles here.
	// That will happen in initialize()
	this->location = glm::vec3(0.0f,0.0f,0.0f);
	this->acceleration = glm::vec3(0.0f,0.0f,0.0f);
	this->m_generateNewParticles = false;
	this->m_timeSinceLastParticle = 0.0f;
	this->m_particleCreationPeriod = FLT_MAX;	// Infinite time until new particles (so won't make any)
	return;
}

void cParticleEmitter::Step(double deltaTime)
{
	// Loop through the particles:
	// - Decrease the life by "deltaTime"
	// - If "still alive" (lifeTime > 0.0):
	//    - Move the particle
	
	for ( auto particle :m_vec_pParticles )
	{
		//cParticle* pCurParticle = *itPart;
		particle->lifeTime -= (float)deltaTime;
		if ( particle->lifeTime > 0.0f )
		{
			particle->velocity += ( this->acceleration * (float)deltaTime );
			particle->location += ( particle->velocity * (float)deltaTime );
			// Change scale. 
			particle->scale *= (1.0f + ((particle->scaleChangePercentPerSecond * (float)deltaTime)));
			// Rotate particle
			glm::quat rotChange = glm::slerp( glm::quat(glm::vec3(0)), 
											particle->qRotationalChangePerSecond, 
											  (float)deltaTime );
			particle->qOrientation *= rotChange;
			particle->colourRGBA.a -= particle->alphaChangePerSecond * (float)deltaTime;
		}
	}
	
	if (this->m_generateNewParticles)
	{
		this->m_timeSinceLastParticle += deltaTime;

		if (this->m_timeSinceLastParticle >= this->m_particleCreationPeriod)
		{
			this->m_CreateNewParticle();
			this->m_timeSinceLastParticle = 0.0f;
		}
	}

	drawParticles();
}

void cParticleEmitter::Initialize(cParticleEmitter::sParticleCreationSettings newParticleSettings)
{
	// In case we are calling Initialize() again... 
	// Clear all the existing particles
	// TODO: 
	// - If the new size is LARGER than the current vector, 
	//   allocate the extra space and call new
	// - If the new size is SMALLER than the current vector, 
	//   go though and call delete on all the "extra" particles. 

	//	this->m_vec_pParticles.clear();
	// Load the vector with the maximum number of particles
	// Reserve doesn't create the objects, it just sets asside space in 1 step
	this->m_vec_pParticles.reserve(cParticleEmitter::DEFAULT_NUM_PARTICLES);

	for (int count = 0; count != cParticleEmitter::DEFAULT_NUM_PARTICLES; count++)
	{
		cParticle* pNewParticle = new cParticle();
		// Set the lifetime to 0.0 for all of them
		pNewParticle->lifeTime = 0.0f;
		this->m_vec_pParticles.push_back(pNewParticle);
	}
	this->m_generateNewParticles = false;
	this->m_NewParticleSettings = newParticleSettings;
	// Calculate now often we should create particles...
	this->m_particleCreationPeriod = 1.0f / (float)this->m_NewParticleSettings.numParticlesPerSecond;
}

void cParticleEmitter::m_CreateNewParticle(void)
{
	// Find the 1st particle that has life <= 0.0f
	for (std::vector<cParticle*>::iterator itPart = this->m_vec_pParticles.begin();
		 itPart != this->m_vec_pParticles.end();  itPart++)
	{
		// Is this particle "dead"? 
		cParticle* pCurParticle = *itPart;

		if (pCurParticle->lifeTime <= 0.0f)
		{
			// Make it "alive" and set to some values
			pCurParticle->lifeTime = randInRange<float>(this->m_NewParticleSettings.minLifeSeconds,
														this->m_NewParticleSettings.maxLifeInSeconds);

			// Set the initial velocity
			pCurParticle->velocity.x = randInRange<float>(this->m_NewParticleSettings.minVelocity.x,
														  this->m_NewParticleSettings.maxVelocity.x);
			pCurParticle->velocity.y = randInRange<float>(this->m_NewParticleSettings.minVelocity.y,
														  this->m_NewParticleSettings.maxVelocity.y);
			pCurParticle->velocity.z = randInRange<float>(this->m_NewParticleSettings.minVelocity.z,
														  this->m_NewParticleSettings.maxVelocity.z);
			//pCurParticle->velocity = randInRange<glm::vec3>(minVelocity, maxVelocity);

			pCurParticle->scale = randInRange<float>(this->m_NewParticleSettings.minStartingScale,
													 this->m_NewParticleSettings.maxStartingScale);

			pCurParticle->scaleChangePercentPerSecond = randInRange<float>(this->m_NewParticleSettings.minScaleChangePercentPerSecond,
																		   this->m_NewParticleSettings.maxScaleChangePercentPerSecond);

			// Use SLERP to pick a rotational change per second
			pCurParticle->qRotationalChangePerSecond = glm::slerp(this->m_NewParticleSettings.minRotationalChangePerSecond,
																  this->m_NewParticleSettings.maxRotationalChangePerSecond,
																  randInRange<float>(0.0f, 1.0f));

			// Pick a location based on the creation volume
			switch (this->m_NewParticleSettings.particleCreationVolume)
			{
			case cParticleEmitter::sParticleCreationSettings::AABB:
				pCurParticle->location.x = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.x,
															  this->m_NewParticleSettings.maxDeltaAABBPosition.x);
				pCurParticle->location.y = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.y,
															  this->m_NewParticleSettings.maxDeltaAABBPosition.y);
				pCurParticle->location.z = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.z,
															  this->m_NewParticleSettings.maxDeltaAABBPosition.z);

				pCurParticle->location += this->location;

				break;
			case cParticleEmitter::sParticleCreationSettings::SPHERE:
				// Pick a random point and check if it's inside a sphere
				bool bKeepPickingPoints = true;
				while (bKeepPickingPoints)
				{
					pCurParticle->location.x = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.x,
																  this->m_NewParticleSettings.maxDeltaAABBPosition.x);
					pCurParticle->location.y = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.y,
																  this->m_NewParticleSettings.maxDeltaAABBPosition.y);
					pCurParticle->location.z = randInRange<float>(this->m_NewParticleSettings.minDeltaAABBPosition.z,
																  this->m_NewParticleSettings.maxDeltaAABBPosition.z);

					if (glm::distance(glm::vec3(0.0f,0.0f,0.0f), pCurParticle->location) >= this->m_NewParticleSettings.minDeltaRadiusPosition)
					{
						if (glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), pCurParticle->location) <= this->m_NewParticleSettings.maxDeltaRadiusPosition)
						{
							// We're good
							pCurParticle->location += this->location;
							bKeepPickingPoints = false;
						}
					}//if (glm::distance...
				
				}//while (bKeepPickingPoints)

				break;
			}
			// TODO: Adjust position based on Max and Min delta postiion
			// (For now, just make the particle the same as the emitter)
			pCurParticle->location = this->location;


			pCurParticle->bImposterFacesCamera = this->m_NewParticleSettings.isImposterFaceCamera;

			pCurParticle->alphaChangePerSecond = 0.0f;
			if ( this->m_NewParticleSettings.bFadeOutOverLifetime )
			{
				// Calculate how fast the alpha value should change
				pCurParticle->alphaChangePerSecond = 1.0f / pCurParticle->lifeTime;
			}

			// All done, so return
			return;

		}//if (pCurParticle->lifeTime <= 0.0f)

	}//for (std::vector<cParticle*>

	// This is if we didn't find any particles to recycle
	return;
}

glm::quat cParticleEmitter::m_calcImposterRotationFromEye(glm::vec3 particlePos, glm::vec3 eye)
{
	// Angle returned is in radian already...
	float angle = atan2(particlePos.x - eye.x,
						particlePos.z - eye.z);

	//* ( 180.0 / PI );
// Since we aren't paying any attention to the x and z rotations.
	return glm::quat( glm::vec3(0.0f, angle, 0.0f) );
}

void cParticleEmitter::getParticles(std::vector<cParticle*>& vec_pParticles, glm::vec3 eyePosition)
{
	// Copy the internal vector of particles to the one that is passed.

	vec_pParticles.clear();
	vec_pParticles.reserve( this->m_vec_pParticles.size() );
//	// Sets the pointer values to zero (NULL)
//	vec_pParticles.resize( this->m_vec_pParticles.size(), 0 );

	// NOTE: There has to be space there, 
	// AND the objects have to be initialized already
//	std::copy( this->m_vec_pParticles.begin(), 
//			   this->m_vec_pParticles.end(), 
//			   vec_pParticles.begin() );

	for (std::vector<cParticle*>::iterator itPart = this->m_vec_pParticles.begin();
		 itPart != this->m_vec_pParticles.end(); itPart++)
	{
		// If alive, copy over
		cParticle* pCurPart = *itPart;

		// Adjust the orientation if it's an imposter. 
		// Have it face the camera
		if (pCurPart->bImposterFacesCamera)
		{
			pCurPart->qOrientation = this->m_calcImposterRotationFromEye(pCurPart->location, eyePosition);
		}

		if (pCurPart->lifeTime > 0.0f)
		{
			vec_pParticles.push_back( pCurPart );
		}
	}
	return;
}

void cParticleEmitter::drawParticles()
{
	cGameObject* pQuadImposter =  ::g_map_GameObjects.at("quadParticle");
	pQuadImposter->isVisible = true;
	// Draw objects at the location of the particles...
	std::vector<cParticle*> vecParticles;
	getParticles( vecParticles, ::g_pFlyCamera->eye);
	for (auto particle : vecParticles)
	{
	    //glm::mat4 matModel = glm::mat4(1.0f);
		pQuadImposter->positionXYZ = particle->location;
		pQuadImposter->scale = particle->scale;
		pQuadImposter->setOrientation( particle->qOrientation );
		pQuadImposter->diffuseColour.a = particle->colourRGBA.a;
		pQuadImposter->alphaTransparency = particle->colourRGBA.a;
		pQuadImposter->tag = "discard";
	    tools::DrawObject(glm::mat4(1), pQuadImposter, shaderProgID, pTheVAOManager);
	}
	pQuadImposter->isVisible = false;
}
