#pragma once
#include "cModelLoader.h"	
#include "cGameObject.h"
#include "cLight.h"
#include "cVAOManager.h"
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