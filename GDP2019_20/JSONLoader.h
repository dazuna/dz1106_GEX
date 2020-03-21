#pragma once

#include "cModelLoader.h"	
#include "cGameObject.h"
#include "cLight.h"
#include "cVAOManager.h"
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"
#include <nlohmann/json.hpp>
#include <iostream>	
#include <fstream>	
#include <vector>
#include <thread>
#include <map>

class JSONLoader
{
public:
	static std::string scene_config;
	static std::string light_json;
	static std::string gameobjects_json;
	static std::string bkp_light_json;
	static std::string bkp_gameobjects_json;
	
	static bool JSONLoadMeshes(
		std::map<std::string, cMesh*>* g_map_Mesh,
		cModelLoader* pTheModelLoader);
	static void loadDefaultMesh(
		std::string filename);
	static bool JSONLoadMeshes(
		std::map<std::string, cMesh*>* g_map_Mesh);
	static bool JSONLoadMeshesSimple();
	static void LoadMeshes_Thread();
	static bool JSONLoadLights(
		std::map<std::string, cLight*>* g_map_pLights,
		GLuint shadProgID);
	static bool JSONLoadGameObjects(
		std::map<std::string, cGameObject*>* g_map_GameObjects);
	static bool loadMeshToGPU(
		cVAOManager* pTheVAOManager,
		std::map<std::string, cGameObject*>* g_map_GameObjects,
		GLuint shaderProgID);
	//static bool JSONSaveLights(std::map<std::string, cLight*>* g_map_pLights);
	static void JSONSaveLights();
	//static bool JSONSaveGameObjects(std::map<std::string, cGameObject*>* g_map_GameObjects);
	static void JSONSaveGos();
	static bool JSONLoadTextures(
		std::map<std::string, cGameObject*>* pGameObjects,
		cBasicTextureManager* pTextureManager);
	static bool JSONLoadSceneConf();
	static bool JSONLoadEntitiesToScene(
		std::map<std::string, cGameObject*>* pGameObjects, 
		std::map<std::string, cLight*>* pLights, cScene* theScene);
	static bool loadSkinnedMesh(nlohmann::json jsonArray, int index);
};

