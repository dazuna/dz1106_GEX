#include "cAnimatedPlayer.h"

cAnimatedPlayer* cAnimatedPlayer::theOneAndOnlyAnimatedPlayer_ = new cAnimatedPlayer();

cAnimatedPlayer* cAnimatedPlayer::getAnimatedPlayer()
{
	return theOneAndOnlyAnimatedPlayer_;
}

void cAnimatedPlayer::addPlayableObject(cGameObject* theGO)
{
	playAnimChars.push_back(theGO);
}

void cAnimatedPlayer::setSpeed(float sp)
{
	this->speed_ = sp;
}

void cAnimatedPlayer::moveAhead()
{
	auto player = *selectedPlayable;
	player->velocity = player->getCurrentAT() * speed_;
	player->pAS->setActiveAnimation("Walk");
}

void cAnimatedPlayer::jump()
{
	auto player = *selectedPlayable;
	player->pAS->setActiveAnimation("Jump");
}

