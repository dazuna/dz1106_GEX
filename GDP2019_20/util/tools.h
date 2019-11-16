#pragma once
#include "../GLCommon.h"
#include "../globalStuff.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>		
#include <sstream>
#include <limits.h>
#include <float.h>
#include <string>
#include "../cModelLoader.h"			
#include "../cVAOManager.h"		// NEW
#include "../cGameObject.h"
#include "../cShaderManager.h"
#include "../TextureManager/cBasicTextureManager.h"
// JSON Stuff
#include "../JSONLoader.h"
// The Physics function
#include "../PhysicsStuff.h"
#include "../cPhysics.h"
#include "../cLowPassFilter.h"
#include "../DebugRenderer/cDebugRenderer.h"
// Used to visualize the attenuation of the lights...
#include "../cLight.h"
#include "../LightManager/cLightHelper.h"
#include "../cFlyCamera/cFlyCamera.h"
// Keyboard, error, mouse, etc. are now here
#include "../GFLW_callbacks.h"

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

extern cFlyCamera* g_pFlyCamera;

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
void DrawObject(glm::mat4 m,cGameObject* pCurrentObject,GLint shaderProgID,cVAOManager* pVAOManager);
bool pFindObjectByFriendlyNameMap(std::string name);
void drawLightXYZ(cDebugRenderer* pDebugRenderer);
void drawGameObjectXYZ(cDebugRenderer* pDebugRenderer);
void drawNormalsXYZ(cDebugRenderer* pDebugRenderer);
std::string GLMvec3toString(glm::vec3 theGLMvec3);
void setWindowTitle(std::stringstream* ssTitle);
void drawPyramidPlayer(cDebugRenderer* pDebugRenderer);
void makeSkullEyesFlicker();
void makeCameraDroneAround(bool isDroneOn);
std::vector<cGameObject*> getWorldMapAsVector();
std::vector<cGameObject*> getClosestTransparentObjectsAsVector();
void printGameObject(cGameObject* theGO);
void makeTransparentObjectsMap();
void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld, cMesh& mesh_transformedInWorld);
