#pragma once
#include <string>
#include <vector>
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

class cAnimationState
{
	public:
	cAnimationState(cSimpleAssimpSkinnedMesh* p_SM);
	
	struct sStateDetails
	{
		sStateDetails():
			currentTime(0.f),
			totalTime(0.f),
			isReplaceable(false){};
	
		std::string name;
		float currentTime;
		float totalTime;
		bool isReplaceable;
		//float frameStepTime;
		// returns true if time had to be reset
		// (for checking to see if animation has finished)
		// todo: deal with runnin the animation backwards
		bool IncrementTime(bool bResetToZero = true);
	};

	cSimpleAssimpSkinnedMesh* pSM;
	std::map<std::string,sStateDetails*> mapAnimationStates;
	sStateDetails* activeAnimation;
	sStateDetails* defaultAnimation;
	float frameTimer;
	bool isReverse;
	
	void loadAnimationDetails(std::string name,bool isReplace=true);
	void makeDefaultAnimation(std::string name);
	void update(float dt,
		std::vector<glm::mat4> &FinalTransformation, 
		std::vector<glm::mat4> &Globals, 
		std::vector<glm::mat4> &Offsets);
	void setActiveAnimation(std::string name, bool shouldReverse = false);
	void stopActiveAnimation();
};
