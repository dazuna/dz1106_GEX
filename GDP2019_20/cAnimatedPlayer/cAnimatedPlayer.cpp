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
	//player->positionXYZ += (player->getCurrentAT() * speed_);
	player->velocity = player->getCurrentAT() * speed_;
	player->pAS->setActiveAnimation("Walk");
}

void cAnimatedPlayer::moveBack()
{
	auto player = *selectedPlayable;
	player->velocity = player->getCurrentAT() * -speed_;
	player->pAS->setActiveAnimation("Walk",true);
}

void cAnimatedPlayer::jump()
{
	auto player = *selectedPlayable;
	player->pAS->setActiveAnimation("Jump");
}

void cAnimatedPlayer::runAhead()
{
	auto player = *selectedPlayable;
	player->velocity = player->getCurrentAT() * (speed_*3.f);
	player->pAS->setActiveAnimation("Run");
}

void cAnimatedPlayer::runBack()
{
	auto player = *selectedPlayable;
	player->velocity = player->getCurrentAT() * -(speed_*3.f);
	player->pAS->setActiveAnimation("Run",true);
}

void cAnimatedPlayer::specialMove()
{
	auto player = *selectedPlayable;
	player->pAS->setActiveAnimation("Attack");
}

void cAnimatedPlayer::leftStrafe()
{
	auto player = *selectedPlayable;
	player->velocity = player->getRight() * -(speed_ * 0.5f);
	player->pAS->setActiveAnimation("leftStrafe");
}

void cAnimatedPlayer::rightStrafe()
{
	auto player = *selectedPlayable;
	player->velocity = player->getRight() * (speed_ * 0.5f);
	player->pAS->setActiveAnimation("rightStrafe");
}

void cAnimatedPlayer::resetVelocity()
{
	auto player = *selectedPlayable;
	player->velocity = glm::vec3(0.f);
}

void cAnimatedPlayer::idle()
{
	auto player = *selectedPlayable;
	player->pAS->stopActiveAnimation();
}

void cAnimatedPlayer::nextPlayer()
{
	auto player = *selectedPlayable;
	player->textures = {"White.bmp","","",""};
	selectedPlayable++;
	if(selectedPlayable == playAnimChars.end()) { selectedPlayable = playAnimChars.begin();}
	player = *selectedPlayable;
	player->textures = {"HotPink.bmp","","",""};
}

void cAnimatedPlayer::rollLeft()
{
	auto player = *selectedPlayable;
	player->velocity = player->getRight() * -speed_;
	player->pAS->setActiveAnimation("rollLeft");
}

void cAnimatedPlayer::rollRight()
{
	auto player = *selectedPlayable;
	player->velocity = player->getRight() * speed_;
	player->pAS->setActiveAnimation("rollRight");
}

