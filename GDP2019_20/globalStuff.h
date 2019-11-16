#pragma once

#include "cGameObject.h"
#include "cLight.h"
#include "cMesh.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "cPhysics.h"
#include "TextureManager/cBasicTextureManager.h"
#include "cModelLoader.h"			
#include "cVAOManager.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

// returns NULL (0) if we didn't find it.
bool pFindObjectByFriendlyNameMap( std::string name );
glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
std::string GLMvec3toString(glm::vec3 theGLMvec3);

// These are defined in theMain.cpp
extern cGameObject* pSkyBox;
extern glm::vec3 cameraEye;
extern glm::vec3 cameraTarget;
extern glm::vec3 visionVector;
extern glm::vec3 upVector;

extern glm::vec3 sexyLightSpotDirection;
extern bool bLightDebugSheresOn;
extern bool debugger;
extern std::string console;

// Load up my "scene" objects (now global)
extern std::map<std::string, cMesh*> g_map_Mesh;
extern std::map<std::string, cGameObject*> g_map_GameObjects;
extern std::map<float, cGameObject*> closestTransparentObjects;
extern std::map<std::string, cGameObject*>::iterator selectedGameObject;
extern std::map<std::string, cLight> g_map_pLights;
extern std::map<std::string, cLight>::iterator selectedLight;

extern enum class selectedType { GAMEOBJECT, LIGHT, SOUND };
extern selectedType cursorType;
extern GLuint shaderProgID;
extern cVAOManager* pTheVAOManager;
extern cModelLoader* pTheModelLoader;
extern cDebugRenderer* pDebugRenderer;
extern cBasicTextureManager* pTextureManager;
extern double timer;
extern bool isDroneOn;