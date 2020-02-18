#include "cGameObject.h"

cGameObject::cGameObject()
{
	this->scale = 1.0f;
	this->isVisible = true;

	this->isWireframe = false;
	this->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->velocity = glm::vec3(0.0f,0.0f,0.0f);
	this->accel = glm::vec3(0.0f,0.0f,0.0f);
	this->inverseMass = 0.0f;	// Infinite mass
	this->physicsShapeType = UNKNOWN;
	this->m_qRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	// Set the unique ID
	// Take the value of the static int, 
	//  set this to the instance variable
	this->m_uniqueID = cGameObject::next_uniqueID;
	// Then increment the static variable
	cGameObject::next_uniqueID++;

	this->disableDepthBufferTest = false;
	this->disableDepthBufferWrite = false;

	this->m_pDebugRenderer = NULL;
	this->alphaTransparency = 1.0f;
// The Skinned Mesh (assimp) object
	//this->pSM = NULL;
	this->pAS = NULL;

	return;
}

cGameObject::cGameObject(cGameObject* newGO)
{
	//  set this to the instance variable
	this->m_uniqueID = cGameObject::next_uniqueID;
	// Then increment the static variable
	cGameObject::next_uniqueID++;

	this->accel = newGO->accel;
	this->debugColour=newGO->debugColour;
	this->diffuseColour=newGO->diffuseColour;
	this->friendlyName=(newGO->friendlyName+"_"+ std::to_string(this->m_uniqueID));
	this->inverseMass=newGO->inverseMass;
	this->isVisible=newGO->isVisible;
	this->isWireframe=newGO->isWireframe;
	this->meshName=newGO->meshName;
	this->meshURL=newGO->meshURL;
	this->objectColourRGBA=newGO->objectColourRGBA;
	this->physicsShapeType=newGO->physicsShapeType;
	this->positionXYZ=newGO->positionXYZ;
	this->setOrientation(newGO->getQOrientation());
	this->scale=newGO->scale;
	this->specularColour=newGO->specularColour;
	this->velocity=newGO->velocity;
	this->alphaTransparency = newGO->alphaTransparency;
	this->textures = newGO->textures;
	this->textureRatio = newGO->textureRatio;
	// The Skinned Mesh (assimp) object
	//this->pSM = NULL;
	this->pAS = NULL;
}

unsigned int cGameObject::getUniqueID(void)
{
	return this->m_uniqueID;
}

void cGameObject::setDebugRenderer(iDebugRenderer* pDebugRenderer)
{
	this->m_pDebugRenderer = pDebugRenderer;
	return;
}

// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cGameObject::next_uniqueID = 1000;	// Starting at 1000, just because


void cGameObject::getVecColliders(std::vector<glm::vec3>* vecColliders)
{
	glm::mat4 transMat = this->getWorldMatrix();
	for (int i = 0; i < this->vecColliders.size(); i++)
	{
		glm::vec4 tempVec4 = glm::vec4(this->vecColliders[i], 1);
		tempVec4 = transMat * tempVec4;
		vecColliders->push_back(tempVec4);
	}
	return;
}

void cGameObject::setVecCollider(std::vector<glm::vec3> vecColliders)
{
	this->vecColliders = vecColliders;
}

glm::mat4 cGameObject::getWorldMatrix()
{
	glm::mat4 matWorld = glm::mat4(1.0f);
	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(this->positionXYZ.x,
				this->positionXYZ.y,
				this->positionXYZ.z));
	matWorld = matWorld * matTrans;
	// ******* TRANSLATION TRANSFORM *********

	// ******* ROTATION TRANSFORM *********
	glm::mat4 matRotation = glm::mat4(this->getQOrientation());
	matWorld = matWorld * matRotation;
	// ******* ROTATION TRANSFORM *********

	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
	glm::vec3(this->scale, this->scale,this->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********
	return matWorld;
}


// //glm::quat m_qRotation;		// Orientation as a quaternion
// glm::quat cGameObject::getQOrientation(void)
// {
// 	return this->m_qRotation;
// }

// // Overwrite the orientation
// void cGameObject::setOrientation(glm::vec3 EulerAngleDegreesXYZ)
// {
// 	// c'tor of the glm quaternion converts Euler 
// 	//	to quaternion representation. 
// 	glm::vec3 EulerAngleRadians;
// 	EulerAngleRadians.x = glm::radians(EulerAngleDegreesXYZ.x);
// 	EulerAngleRadians.y = glm::radians(EulerAngleDegreesXYZ.y);
// 	EulerAngleRadians.z = glm::radians(EulerAngleDegreesXYZ.z);

// 	this->m_qRotation = glm::quat(EulerAngleRadians);
// }

// void cGameObject::setOrientation(glm::quat qAngle)
// {
// 	this->m_qRotation = qAngle;
// }

// // Updates the existing angle
// void cGameObject::updateOrientation(glm::vec3 EulerAngleDegreesXYZ)
// {
// 	glm::vec3 EulerAngleRadians;
// 	EulerAngleRadians.x = glm::radians(EulerAngleDegreesXYZ.x);
// 	EulerAngleRadians.y = glm::radians(EulerAngleDegreesXYZ.y);
// 	EulerAngleRadians.z = glm::radians(EulerAngleDegreesXYZ.z);

// 	glm::quat angleChange = glm::quat(EulerAngleRadians);

// 	this->m_qRotation *= angleChange;
// }

// void cGameObject::updateOrientation(glm::quat qAngleChange)
// {
// 	this->m_qRotation *= qAngleChange;
// }

// glm::vec3 cGameObject::getEulerAngle(void)
// {
// 	// In glm::gtx (a bunch of helpful things there)
// 	glm::vec3 EulerAngle = glm::eulerAngles(this->m_qRotation);
// 	EulerAngle = glm::degrees(EulerAngle);
// 	return EulerAngle;
// }