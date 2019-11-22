#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include "../cGameObject.h"
#include "../DebugRenderer/cDebugRenderer.h"	

extern cDebugRenderer* pDebugRenderer;

class playerController
{
public:
	playerController();
	playerController(cGameObject* pGO);
	// Use these to allow us to use the LookAt() transform
	glm::vec3 position;
	cGameObject* pPlayer;

private:
	glm::vec3 m_at;			// A direction, not a location
	glm::vec3 m_up;
	// THESE DON'T LIKELY CHANGE
	glm::vec3 m_front;// = glm::vec3(0,0,1);
	glm::vec3 m_upIsYVector;	// = glm::vec3(0,1,0);
public:
	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getDirection(void);
	glm::vec3 getUpVector(void);
public:

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);
	void Pitch_UpDown(float angleDegrees);	// around X
	void Yaw_LeftRight(float angleDegrees);	// around y
	void Roll_CW_CCW(float angleDegrees);	// around z

	glm::quat getQOrientation(void) { return this->qOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	void drawSelf();
	void updatePosition(glm::vec3 newPosition);

private:
	// This will lead to direction, etc.
	glm::quat qOrientation;
	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);


};
