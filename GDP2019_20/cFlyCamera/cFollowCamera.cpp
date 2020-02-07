#include "cFollowCamera.h"
#include "../globalStuff.h"


glm::vec3 cFollowCamera::getTargetPos()
{
	if (!viewableObjects.empty())
	{
		glm::mat4 targetTransform;
		(*currentObject)->physicsComponent->GetTransform(targetTransform);
		return glm::vec3(targetTransform[3].x, targetTransform[3].y, targetTransform[3].z);
	}
	else
	{
		return glm::vec3(0);
	}
}

void cFollowCamera::init(glm::vec3 offset)
{
	this->offset = offset;
	
	viewableObjects.clear();
	
	for (auto i = ::g_map_GameObjects.begin();
		i != ::g_map_GameObjects.end(); i++)
	{
		cGameObject* theGO = i->second;
		if (theGO->physicsComponent &&
			theGO->physicsComponent->GetComponentType() == nPhysics::eComponentType::ball)
		{
			viewableObjects.push_back(theGO);
		}
	}
	if (viewableObjects.empty())
	{
		return;
	}
	currentObject = viewableObjects.begin();
	auto colorChanger = *currentObject;
	//set second texture which is red to movable sphere;
	colorChanger->textureRatio = { 0.f,1.f,0.f,0.f };
	setTarget(getTargetPos());
	update();
}

void cFollowCamera::moveUp(float angle)
{
	offset = glm::rotate(getDirection(), -angle, getRight());
	update();
}

void cFollowCamera::moveDown(float angle)
{
	offset = glm::rotate(getDirection(), angle, getRight());
	update();
}

void cFollowCamera::moveRight(float angle)
{
	offset = glm::rotate(getDirection(), angle, upVector);
	update();
}

void cFollowCamera::moveLeft(float angle)
{
	offset = glm::rotate(getDirection(), -angle, upVector);
	update();
}

void cFollowCamera::zoom(float step)
{
	offset *= step;
	update();
}

void cFollowCamera::setTarget(glm::vec3 target)
{
	this->target = target;
}

glm::vec3 cFollowCamera::getRight()
{
	return glm::cross(this->getUpVector(),this->getCameraDirection());
}

 glm::vec3 cFollowCamera::getDirection()
{
	 return this->eye - this->target;
}

void cFollowCamera::update()
{
	//printf("itemTarget pos: %s\nOffset: %s\n", 
	//	glm::to_string(mTargetComponent->getPos()).c_str(), 
	//	glm::to_string(offset).c_str());
	glm::vec3 targetPos = getTargetPos();
	setTarget(targetPos);
	this->eye = (targetPos + offset);
	this->cameraLookAt(targetPos);
}

void cFollowCamera::pushLeft()
{
	auto theGO = *currentObject;
	auto left = (getRight()) * 15.f;
	theGO->physicsComponent->ApplyForce(left);
}

void cFollowCamera::pushRight()
{
	auto theGO = *currentObject;
	auto right = (-getRight()) * 15.f;
	theGO->physicsComponent->ApplyForce(right);
}

void cFollowCamera::pushUp()
{
	auto theGO = *currentObject;
	auto forward = -offset;
	forward.y = 0.f;
	theGO->physicsComponent->ApplyForce(forward);
}

void cFollowCamera::pushDown()
{
	auto theGO = *currentObject;
	auto backward = offset;
	backward.y = 0.f;
	theGO->physicsComponent->ApplyForce(backward);
}

void cFollowCamera::pushAscend()
{
	auto theGO = *currentObject;
	theGO->physicsComponent->ApplyForce(glm::vec3(0,30,0));
}

void cFollowCamera::nextViewableObject()
{
	auto colorChanger = *currentObject;
	colorChanger->textureRatio = { 1.f,0.f,0.f,0.f };
	++currentObject;
	if(currentObject == viewableObjects.end())
	{
		currentObject = viewableObjects.begin();
	}
	colorChanger = *currentObject;
	//set second texture which is red to movable sphere;
	colorChanger->textureRatio = { 0.f,1.f,0.f,0.f };
}
