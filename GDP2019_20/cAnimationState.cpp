#include "cAnimationState.h"
#include <iostream>

cAnimationState::cAnimationState(cSimpleAssimpSkinnedMesh* p_SM)
{
	this->pSM = p_SM;
	frameTimer = 0.f;
	isReverse = false;
	activeAnimation = nullptr;
	defaultAnimation = nullptr;
}

bool cAnimationState::sStateDetails::IncrementTime(bool bResetToZero)
{
	return false;
}

void cAnimationState::loadAnimationDetails(std::string name, bool isReplace)
{
	auto pSD = new sStateDetails();
	pSD->name = name;
	pSD->isReplaceable = isReplace;
	// get total time in seconds
	pSD->totalTime = pSM->FindAnimationTotalTime(name);	
	const auto x = this->pSM->mapAnimationFriendlyNameTo_pScene.at(name).pAIScene->mAnimations[0];
	const auto TicksPerSecond = float(x->mTicksPerSecond);
	pSD->totalTime = pSD->totalTime/TicksPerSecond;
	
	mapAnimationStates.insert({name,pSD});
	//std::cout << "loaded animState: " << name << std::endl;
}

void cAnimationState::makeDefaultAnimation(std::string name)
{
	if(mapAnimationStates.find(name) != mapAnimationStates.end()) // animation exists
	{
		this->defaultAnimation = mapAnimationStates.at(name);
		std::cout << "default animState: " << name << std::endl;
	}
}

void cAnimationState::update(float dt,std::vector<glm::mat4> &FinalTransformation,std::vector<glm::mat4> &Globals,std::vector<glm::mat4> &Offsets)
{
	
	if(!activeAnimation)
	{
		pSM->BoneTransform( frameTimer,	// 0.0f // Frame time
					    defaultAnimation->name,
						FinalTransformation, 
						Globals, 
					    Offsets );
	}
	else
	{
		auto actualTimer = frameTimer;
		if(isReverse) actualTimer = (activeAnimation->totalTime - frameTimer);
		pSM->BoneTransform( actualTimer,	// 0.0f // Frame time
					    activeAnimation->name,
						FinalTransformation, 
						Globals, 
					    Offsets );
		//std::cout << "frameTimer: " << frameTimer << " total_time: " << activeAnimation->totalTime << std::endl;
		if(frameTimer >= activeAnimation->totalTime)
		{
			activeAnimation = nullptr;
			frameTimer = 0.f;
			isReverse = false;
		}
	}


	
	// make things move...
	this->frameTimer += dt;
}

void cAnimationState::setActiveAnimation(std::string name, bool shouldReverse)
{	
	if (!activeAnimation)
	{
		if(mapAnimationStates.find(name)!=mapAnimationStates.end())
		{
			this->frameTimer = 0.f;
			activeAnimation = mapAnimationStates.at(name);
			if (shouldReverse) isReverse = true;
		}
	}
	else
	{
		if(activeAnimation->isReplaceable)
		{
			if(mapAnimationStates.find(name)!=mapAnimationStates.end())
			{
				if(activeAnimation->name != name)
				{
					this->frameTimer = 0.f;
					activeAnimation = mapAnimationStates.at(name);
					if (shouldReverse) isReverse = true;
				}
			}
		}
	}
}

void cAnimationState::stopActiveAnimation()
{
	if(activeAnimation)
	{
		this->activeAnimation = nullptr;
		this->frameTimer = 0.f;
		this->isReverse = false;
	}
}