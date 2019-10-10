#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

// NOTE: We are including the INTERFACE, not the actual renderer
#include "DebugRenderer/iDebugRenderer.h"

enum eShapeTypes
{
	AABB,					// min and max corners 
	SPHERE,					// Radius
	CAPSULE,			
	PLANE,
	MESH,		// Triangle test
	UNKNOWN
};

class cGameObject
{
public:
	cGameObject();
	// Used to draw this mesh
	std::string meshName;
	std::string meshURL;

	// Values that we can assign and then look for them
	unsigned int friendlyIDNumber;
	std::string friendlyName;

	glm::vec3  positionXYZ;
	glm::vec3  rotationXYZ;
	float scale;;   

	glm::mat4 matWorld;
	glm::vec4  objectColourRGBA;	
	// Likely want this now:
	glm::vec4 diffuseColour;		// Colour of the object
	glm::vec4 specularColour;		// Colour of the HIGHLIGHT + Shininess
	                                // RGB - colour
	                                // 4th value (a or w) is the "power"
	                                // 1.0 to 10,000.0f 


	// Add some physics things
	glm::vec3 velocity;
	glm::vec3 accel;

	// For driving the ship around
	float HACK_speed;
	float HACK_AngleAroundYAxis;		// Angle, around the y axis


	// If the object has an inverse mass of 0.0
	//	then it's not updated by the physics code
	float inverseMass;	// 0.0f = Doesn't move
	//bool bIsDynamic;
	// We could contain another class or struct with the phsyics stuff
	// or we could use this type to determine what we need to check.
	eShapeTypes physicsShapeType;
	glm::vec3 AABB_min, AABB_max;
	float SPHERE_radius;
	// Won't be lit, and is wireframe
	bool isWireframe;
	glm::vec4 debugColour;
	bool isVisible;
	bool disableDepthBufferTest;
	bool disableDepthBufferWrite;

	unsigned int getUniqueID(void);
	void setDebugRenderer(iDebugRenderer* pDebugRenderer);

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
	iDebugRenderer* m_pDebugRenderer;
};

#endif


