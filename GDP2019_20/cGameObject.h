#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <iostream>

// NOTE: We are including the INTERFACE, not the actual renderer
#include "DebugRenderer/iDebugRenderer.h"
#include "bRotatable/bRotatable.hpp"
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
#include "cAnimationState.h"

enum eShapeTypes
{
	AABB,					// min and max corners 
	SPHERE,					// Radius
	CAPSULE,			
	PLANE,
	MESH,		// Triangle test
	UNKNOWN
};

class cGameObject : public bRotatable
{
public:
	cGameObject();
	cGameObject(cGameObject* newGO);
	// Used to draw this mesh
	std::string meshName;
	std::string meshURL;
	// Values that we can assign and then look for them
	unsigned int friendlyIDNumber;
	std::string friendlyName;
	glm::vec3  positionXYZ;
	// glm::vec3  rotationXYZ;
	glm::vec4  objectColourRGBA;
	float alphaTransparency;
	glm::vec4 diffuseColour;		
	glm::vec4 specularColour;
	float scale;	
	glm::mat4 matWorld;
	// Add some physics things
	glm::vec3 velocity;
	glm::vec3 accel;
	// Add some textures to each obj
	static const int NUMBEROFTEXTURES = 4;
	std::vector<std::string> textures;
	std::vector<float> textureRatio;
	// stuff for animation
	std::string tag = "";
	int lifetime = 0;
	// If the object has an inverse mass of 0.0	//	then it's not updated by the physics code
	float inverseMass;
	eShapeTypes physicsShapeType;
	glm::vec3 AABB_min, AABB_max;
	float SPHERE_radius;
	// Won't be lit, and is wireframe
	bool isWireframe;
	glm::vec4 debugColour;
	bool isVisible;
	bool disableDepthBufferTest;
	bool disableDepthBufferWrite;

	//cSimpleAssimpSkinnedMesh* pSM;
	cAnimationState* pAS;

public:
	// glm::quat getQOrientation(void);
	// // Overwrite the orientation
	// void setOrientation(glm::vec3 EulerAngleDegreesXYZ);
	// void setOrientation(glm::quat qAngle);
	// // Updates the existing angle
	// void updateOrientation(glm::vec3 EulerAngleDegreesXYZ);
	// void updateOrientation(glm::quat qAngle);
	// glm::vec3 getEulerAngle(void);
	void getVecColliders(std::vector<glm::vec3>* vecColliders);
	void setVecCollider(std::vector<glm::vec3> vecColliders);
	glm::mat4 getWorldMatrix();
	// ID
	unsigned int getUniqueID(void);
	void setDebugRenderer(iDebugRenderer* pDebugRenderer);

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
	iDebugRenderer* m_pDebugRenderer;
	glm::quat m_qRotation;		// Orientation as a quaternion
	std::vector<glm::vec3> vecColliders;
};

#endif


