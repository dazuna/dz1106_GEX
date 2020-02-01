#pragma once
#include "cModelLoader.h"	
#include "cGameObject.h"
#include "cLight.h"
#include "cVAOManager.h"
#include "globalStuff.h"
#include "util/tools.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>
#include <iostream>	
#include <fstream>	
#include <vector>
#include <map>

bool JSONLoadMeshes(
	std::map<std::string, cMesh*>* g_map_Mesh,
	cModelLoader* pTheModelLoader);
bool JSONLoadLights(
	std::map<std::string, cLight>* g_map_pLights,
	GLuint shadProgID);
bool JSONLoadGameObjects(
	std::map<std::string, cGameObject*>* g_map_GameObjects);
bool loadMeshToGPU(
	cVAOManager* pTheVAOManager,
	std::map<std::string, cMesh*>* g_map_Mesh,
	std::map<std::string, cGameObject*>* g_map_GameObjects,
	GLuint shaderProgID);
bool JSONSaveLights(
	std::map<std::string, cLight>* g_map_pLights);
bool JSONSaveGameObjects(
	std::map<std::string, cGameObject*>* g_map_GameObjects);
bool JSONLoadTextures();
void SetUpTextureBindingsForObject(
	cGameObject* pCurrentObject,
	GLint shaderProgID);

// using the factory of my wrapper to create the correspondin
// physics component which hinges on the gameObject shapeType
// that I originally had implemented for physics1
nPhysics::iPhysicsComponent* createPhysicsComponent(cGameObject* theGO);