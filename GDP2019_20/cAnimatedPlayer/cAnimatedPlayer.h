#pragma once
#include "../cGameObject.h"

class cAnimatedPlayer
{
	static cAnimatedPlayer* theOneAndOnlyAnimatedPlayer_;
	cAnimatedPlayer():speed_(20.f), isPlayModeOn(false){}
	
	float speed_;
public:
	static cAnimatedPlayer* getAnimatedPlayer();
	
    std::vector<cGameObject*> playAnimChars{};
    std::vector<cGameObject*>::iterator selectedPlayable;
	bool isPlayModeOn;
	
    void addPlayableObject(cGameObject* theGO);
	void setSpeed(float sp);
	void moveAhead();
	void moveBack();
	void runAhead();
	void runBack();
	void leftStrafe();
	void rightStrafe();
	void rollLeft();
	void rollRight();
	void resetVelocity();
	void jump();
	void specialMove();
	void idle();
	void nextPlayer();
};
