#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include <float.h>
#include <string>		
#include "../cVAOManager.h"		// NEW
#include "../cGameObject.h"
#include "../cShaderManager.h"
#include "../TextureManager/cBasicTextureManager.h"
#include "../globalStuff.h"
#include "../cAABB/cAABB.h"
#include "../DebugRenderer/cDebugRenderer.h"
#include "../cFlyCamera/cFlyCamera.h"
#include <nlohmann/json.hpp>

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

extern cFlyCamera* g_pFlyCamera;
extern std::map<unsigned long long /*ID*/, cAABB*> g_mapAABBs_World;
extern double averageDeltaTime;

class tools
{
public:
	static glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
	static void DrawObject(glm::mat4 m, cGameObject* pCurrentObject, GLint shaderProgID, cVAOManager* pVAOManager);
	static bool pFindObjectByFriendlyNameMap(std::string name);
	static bool pFindLightByNameMap(std::string name);
	static void drawLightXYZ(cDebugRenderer* pDebugRenderer);
	static void drawGameObjectXYZ(cDebugRenderer* pDebugRenderer);
	static void drawNormalsXYZ(cDebugRenderer* pDebugRenderer);
	static std::string GLMvec3toString(glm::vec3 theGLMvec3);
	static void setWindowTitle(std::stringstream* ssTitle);
	static void drawPyramidPlayer(cDebugRenderer* pDebugRenderer);
	static void makeSkullEyesFlicker();
	static void makeCameraDroneAround(bool isDroneOn);
	static std::vector<cGameObject*> getWorldMapAsVector();
	static std::vector<cGameObject*> getClosestTransparentObjectsAsVector();
	static void printGameObject(cGameObject* theGO);
	static void makeTransparentObjectsMap();
	//static void makeTransparentObjectsMap(std::map<std::string, cGameObject*> mapGameObject);
	static void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld, cMesh& mesh_transformedInWorld);
	static void drawAABBs();
	static void drawSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime);
	static void duplicateSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime);
	static void lifetimeValidation(cGameObject* pCurObject);
	static void SetUpTextureBindingsForObject(cGameObject* pCurrentObject,GLint shaderProgID);
	// find something in any map
	template <typename T, typename K>
	static bool mapContains(std::map<K, T> mapa, K llave)
	{ return mapa.find(llave) != mapa.end(); }
};
