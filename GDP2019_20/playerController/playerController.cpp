#include "playerController.h"

//#include <glm/glm.hpp>
//#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

playerController::playerController()
{
	this->position = glm::vec3(0.0f);
	// This will be constant
	this->m_front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
	return;
}

playerController::playerController(cGameObject* pGO)
{
	this->position = glm::vec3(0.0f);
	this->pPlayer = pGO;
	this->position = pGO->positionXYZ;
	this->pPlayer->tag = "player";
	// This will be constant
	this->m_front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
	return;
}

glm::vec3 playerController::getAtInWorldSpace(void)
{
	// The "At" is relative to the where the camera is...
	// So just add the "At" to the current camera location
	return this->pPlayer->positionXYZ + this->m_at;
}

glm::vec3 playerController::getDirection(void)
{
	this->m_UpdateAtFromOrientation();

	return this->m_at;
}

glm::vec3 playerController::getUpVector(void)
{
	return this->m_up;
}

void playerController::MoveForward_Z(float amount)
{
	//glm::vec3 direction = this->getAtInWorldSpace() - this->position;
	glm::vec3 direction = this->getAtInWorldSpace() - this->pPlayer->positionXYZ;

	direction = glm::normalize(direction);
	glm::vec3 amountToMove = direction * amount;

	//this->position += amountToMove;
	this->pPlayer->velocity += amountToMove;
	return;
}

void playerController::MoveUpDown_Y(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	//this->position += vecAmountToMove;
	this->pPlayer->velocity += vecAmountToMove;
	return;
}

void playerController::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getDirection(), this->getUpVector());
	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;
	//this->position += vecAmountToMove;
	this->pPlayer->velocity += vecAmountToMove;
	return;
}

void playerController::Pitch_UpDown(float angleDegrees)	// around X
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void playerController::Yaw_LeftRight(float angleDegrees)	// around y
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void playerController::Roll_CW_CCW(float angleDegrees)	// around z
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void playerController::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
								   glm::radians(newAnglesEuler.y),
								   glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void playerController::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void playerController::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
								  glm::radians(adjAngleEuler.y),
								  glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	this->pPlayer->updateOrientation(rotationAdjust);
	this->pPlayer->velocity = glm::rotate(rotationAdjust, this->pPlayer->velocity);
	return;
}

void playerController::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void playerController::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void playerController::m_UpdateAtFromOrientation(void)
{
	glm::mat4 matRotation = glm::mat4(this->qOrientation);
	glm::vec4 frontOfCamera = glm::vec4(this->m_front, 1.0f);
	glm::vec4 newAt = matRotation * frontOfCamera;
	this->m_at = glm::vec3(newAt);
	return;
}

void playerController::m_UpdateUpFromOrientation(void)
{
	glm::mat4 matRotation = glm::mat4(this->qOrientation);
	glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);
	glm::vec4 newUp = matRotation * upVector;
	this->m_up = glm::vec3(newUp);
}

void playerController::drawSelf()
{
	pDebugRenderer->addLine(this->position, this->getAtInWorldSpace(), glm::vec3(0, 0, 1));
}

void playerController::updatePosition(glm::vec3 newPosition)
{
	this->position = newPosition;
	this->pPlayer->positionXYZ = newPosition;
}

