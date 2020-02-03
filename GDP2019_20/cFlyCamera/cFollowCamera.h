#pragma once

#include "cFlyCamera.h"
#include <game_math.h>
#include "physics/interfaces/iPhysicsComponent.h"
#include "../cGameObject.h"
#include <glm/gtx/rotate_vector.hpp>

class cFollowCamera : public cFlyCamera
{
private:
	static cFollowCamera* theCamera;
	glm::vec3 offset;
	std::vector<cGameObject*> viewableObjects;
	std::vector<cGameObject*>::iterator currentObject;
	glm::vec3 getTargetPos();
	glm::vec3 target;
public:
	void init(glm::vec3 offset);
	void moveUp(float angle);
	void moveDown(float angle);
	void moveRight(float angle);
	void moveLeft(float angle);
	void zoom(float step);
	void setTarget(glm::vec3 target);
	inline glm::vec3 getRight();
	inline glm::vec3 getDirection();
	/*
		Repositions the camera in the offset of the target.
	*/
	void update();
	// apply forces to spheres
	void pushLeft();
	void pushRight();
	void pushUp();
	void pushDown();
	void pushAscend();
	void nextViewableObject();
};