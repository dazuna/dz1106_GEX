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
	this->rotationXYZ=newGO->rotationXYZ;
	this->scale=newGO->scale;
	this->specularColour=newGO->specularColour;
	this->velocity=newGO->velocity;
	this->alphaTransparency = newGO->alphaTransparency;
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
