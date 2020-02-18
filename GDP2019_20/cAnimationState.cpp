#include "cAnimationState.h"
#include <iostream>

cAnimationState::cAnimationState(cSimpleAssimpSkinnedMesh* p_SM)
{
	this->pSM = p_SM;
	frameTimer = 0.f;
	activeAnimation = nullptr;
	defaultAnimation = nullptr;
}

bool cAnimationState::sStateDetails::IncrementTime(bool bResetToZero)
{
	return false;
}

void cAnimationState::loadAnimationDetails(std::string name)
{
	auto pSD = new sStateDetails();
	pSD->name = name;
	pSD->totalTime = pSM->FindAnimationTotalTime(name);
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
		pSM->BoneTransform( frameTimer,	// 0.0f // Frame time
					    activeAnimation->name,
						FinalTransformation, 
						Globals, 
					    Offsets );
		if(frameTimer == activeAnimation->totalTime)
		{
			activeAnimation = nullptr;
			frameTimer = 0.f;
		}
	}
	

	// make things move...
	this->frameTimer += dt;
}
