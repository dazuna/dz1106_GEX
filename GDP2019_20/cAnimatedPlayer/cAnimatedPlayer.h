#pragma once
#include "../cGameObject.h"

class cAnimatedPlayer
{
	static cAnimatedPlayer* theOneAndOnlyAnimatedPlayer_;
	cAnimatedPlayer():speed_(10.f), isPlayModeOn(false){}
	
	float speed_;
public:
	static cAnimatedPlayer* getAnimatedPlayer();
	
    std::vector<cGameObject*> playAnimChars{};
    std::vector<cGameObject*>::iterator selectedPlayable;
	bool isPlayModeOn;
	
    void addPlayableObject(cGameObject* theGO);
	void setSpeed(float sp);
	void moveAhead();
	void jump();
};
