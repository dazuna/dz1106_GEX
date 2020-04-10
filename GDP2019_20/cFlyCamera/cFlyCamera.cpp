#include "cFlyCamera.h"
#include "../steeringBehaviour/cSteeringBehaviour.hpp"

//#include <glm/glm.hpp>
//#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "../util/tools.h"

cFlyCamera* cFlyCamera::the_camera_ = new cFlyCamera();

cFlyCamera* cFlyCamera::getTheCamera()
{
	return the_camera_;
}

cFlyCamera::cFlyCamera()
{
	this->eye = glm::vec3(0,10,-10);
	// This will be constant
	this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->movementSpeed = 4.0f;
	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;
	this->m_Mouse_Initial_State_Is_Set = false;
	this->bKeepCameraFacingUp = true;

	// battle camera stuffs
	this->offsetFromBattle = glm::vec3(0,20,30);
	this->state = "normal";
	this->timer = 0.f;
	this->waitTimer = 0.f;
	
	return;
}

cFlyCamera::cFlyCamera(glm::vec3 visionVec3)
{
	this->eye = glm::vec3(0.0f);

	// This will be constant
	this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->movementSpeed = 4.0f;

	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	//this->qOrientation = glm::quat(visionVec3);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	//// Initial "at" is 1 unit in front of the camera, along z
	//this->at = this->m_frontOfCamera;

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;

	this->m_Mouse_Initial_State_Is_Set = false;

	this->bKeepCameraFacingUp = true;

	return;
}

glm::vec3 cFlyCamera::getAtInWorldSpace(void)
{
	// The "At" is relative to the where the camera is...
	// So just add the "At" to the current camera location
	return this->eye + this->m_at;
}

glm::vec3 cFlyCamera::getCameraDirection(void)
{
	this->m_UpdateAtFromOrientation();

	return this->m_at;
}

void cFlyCamera::MoveForward_Z(float amount)
{
	// We take the vector from the at to eye (i.e. direction)
	// Add this forward velocity along this direction.

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;

	return;
}

void cFlyCamera::MoveUpDown_Y(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cFlyCamera::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cFlyCamera::Pitch_UpDown(float angleDegrees)	// around X
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Yaw_LeftRight(float angleDegrees)	// around y
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Roll_CW_CCW(float angleDegrees)	// around z
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
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

void cFlyCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
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

	return;
}

void cFlyCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cFlyCamera::m_UpdateAtFromOrientation(void)
{
	// Have a rotation around the origin (eye)

	// vec4 new = mat4Transform * (vec4)Location

	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->m_frontOfCamera, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->m_at = glm::vec3(newAt);


	return;
}

void cFlyCamera::m_UpdateUpFromOrientation(void)
{
	if (this->bKeepCameraFacingUp)
	{
		this->m_up = this->m_upIsYVector;
	}
	else
	{
		glm::mat4 matRotation = glm::mat4(this->qOrientation);

		glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);

		glm::vec4 newUp = matRotation * upVector;

		// Update the "At"
		this->m_up = glm::vec3(newUp);
	}

	return;
}

glm::vec3 cFlyCamera::getUpVector(void)
{
	return this->m_up;
}

void cFlyCamera::setMouseXY(double newX, double newY)
{
	this->m_lastMouse_X = this->m_Mouse_X;
	this->m_Mouse_X = (float)newX;

	this->m_lastMouse_Y = this->m_Mouse_Y;
	this->m_Mouse_Y = (float)newY;

	// Can return value numbers
	this->m_Mouse_Initial_State_Is_Set = true;

	return;
}

void cFlyCamera::setMouseWheelDelta(double deltaWheel)
{
	this->m_lastMouse_Wheel = this->m_MouseWheel;
	this->m_MouseWheel += (float)deltaWheel;
	return;
}

float cFlyCamera::getMouseX(void)
{
	return this->m_Mouse_X;
}

float cFlyCamera::getMouseY(void)
{
	return this->m_Mouse_Y;
}

float cFlyCamera::getDeltaMouseX(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_X - this->m_lastMouse_X;
}

float cFlyCamera::getDeltaMouseY(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_Y - this->m_lastMouse_Y;
}

float cFlyCamera::getMouseWheel(void)
{
	return this->m_MouseWheel;
}

void cFlyCamera::followPlayer(cGameObject* cPlayer, bool isFollowON)
{
	if (!isFollowON) { return; } // Do not follow player

	glm::vec3 origin = cPlayer->positionXYZ;
	glm::vec3 newAt = glm::vec3(1, 0, 0);
	newAt *= 200.0f;
	this->eye = newAt + origin;

	return;
}

void cFlyCamera::watchPlayer(playerController* cPlayer)
{
	glm::quat orientation = glm::conjugate(glm::toQuat(glm::lookAt(cPlayer->pPlayer->positionXYZ, this->eye, this->getUpVector())));
	this->qOrientation = orientation;
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
	return;
}

void cFlyCamera::cameraLookAt(glm::vec3 target)
{
	glm::quat orientation = glm::conjugate(glm::toQuat(glm::lookAt(target, this->eye, this->getUpVector())));
	this->qOrientation = orientation;
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
	return;
}

void cFlyCamera::battleCamera()
{
	if(state != "zoom_in") { return; }
	auto desiredPosition = battleTarget+offsetFromBattle;
	auto dist = glm::distance(desiredPosition,camPos->positionXYZ);
	if(dist < 1.f){ state = "normal"; return; }
	camPos->velocity = cSteeringBehaviour::arriveBhvr(
		camPos->positionXYZ,camPos->velocity,
		desiredPosition,0.3f,40.f);
	battlePos->velocity = cSteeringBehaviour::arriveBhvr(
		battlePos->positionXYZ,battlePos->velocity,
		battleTarget,0.3f,50.f);
	this->eye = camPos->positionXYZ;
	cameraLookAt(battlePos->positionXYZ);
}

void cFlyCamera::zoomOutCamera()
{
	if(state != "zoom_out") { return; }
	auto desiredPosition = battleTarget+offsetFromBattle+glm::vec3(0,50,50);
	auto dist = glm::distance(desiredPosition,camPos->positionXYZ);	
	if(dist < 1.f){ state = "normal"; return; }
	camPos->velocity = cSteeringBehaviour::arriveBhvr(
		camPos->positionXYZ,camPos->velocity,
		desiredPosition,0.3f,35.f);	
	this->eye = camPos->positionXYZ;
}

void cFlyCamera::normalCamera()
{	
	if(state != "normal") { return; }
	camPos->positionXYZ = this->eye;
	battlePos->positionXYZ = this->getAtInWorldSpace();
}

void cFlyCamera::waitCamera(float dt)
{
	if(state != "wait") return;
	camPos->positionXYZ = this->eye;
	battlePos->positionXYZ = this->getAtInWorldSpace();
	timer += dt;
	if(timer > 1.f)
	{
		timer = 0.f;
		waitTimer += 1.f;
	}
	if(waitTimer >= timeToWait)
	{
		waitTimer = 0.f;
		state = nextAction;
	}	
}

void cFlyCamera::gameCameraUpdate(float dt)
{
	normalCamera();
	battleCamera();
	zoomOutCamera();
	waitCamera(dt);
}

void cFlyCamera::repositionMiniMap()
{
	if (tools::pFindObjectByFriendlyNameMap("miniMapQuad"))
	{
		auto mapQuad = ::g_map_GameObjects["miniMapQuad"];
		auto left = glm::normalize(glm::cross(this->m_up, this->m_at));
		auto litled_down = glm::normalize(glm::cross(left, this->m_at));
		mapQuad->positionXYZ = this->eye
			+ this->m_at * 5.f // in front of the camera
			+ litled_down * 1.0f // a little down
			+ left * 2.2f; //a little left
		mapQuad->setOrientation(LookAt(-this->m_at, { 0,1,0 }));
	}
}

