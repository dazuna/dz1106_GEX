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

class JSONLoader
{
public:
	static std::string light_json;
	static std::string gameobjects_json;
	static std::string bkp_light_json;
	static std::string bkp_gameobjects_json;
	
	static bool JSONLoadMeshes(
		std::map<std::string, cMesh*>* g_map_Mesh,
		cModelLoader* pTheModelLoader);
	static bool JSONLoadLights(
		std::map<std::string, cLight>* g_map_pLights,
		GLuint shadProgID);
	static bool JSONLoadGameObjects(
		std::map<std::string, cGameObject*>* g_map_GameObjects);
	static bool loadMeshToGPU(
		cVAOManager* pTheVAOManager,
		std::map<std::string, cMesh*>* g_map_Mesh,
		std::map<std::string, cGameObject*>* g_map_GameObjects,
		GLuint shaderProgID);
	static bool JSONSaveLights(
		std::map<std::string, cLight>* g_map_pLights);
	static bool JSONSaveGameObjects(
		std::map<std::string, cGameObject*>* g_map_GameObjects);
	static bool JSONLoadTextures();
};