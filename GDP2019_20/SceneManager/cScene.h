#pragma once
#include <string>
#include <map>
#include "../cFBO/cFBO.h"
#include "../cGameObject.h"
#include "../cLight.h"
#include "../cFlyCamera/cFlyCamera.h"

/*{
    "name": "scene_1",
    "jsonPath": "configScenes/scene1.json",
    "effect": "none",
    "cameraNumber": 0,
    "width": 1280,
    "height": 720
}*/

class cScene
{
public:
	enum class sceneEffect
	{
		none,
		bloom,
		black_white,
		night_vision,
		blur
	};
	
    ~cScene();
	cScene() = default;
    cScene(std::string name_,std::string jsonPath_, const std::string& stringEffect_,
		std::vector<int> camIndex_,int sceneWidth_,int sceneHeight_);
	
	// properties
	std::map<std::string,cGameObject*> sceneGameObjects;
	std::map<std::string,cLight*> sceneLights;
	std::vector<cFlyCamera*> cameras;
	cFBO* theFBO;
	GLuint* shaderProgramId;

	std::string name;
	std::string jsonPath;
	std::string stringEffect;
	sceneEffect effect;
	std::vector<int> camIndex;
	int sceneWidth;
	int sceneHeight;
	
	static sceneEffect convertStrToSceneEffect(const std::string& string);
	// shader ID
	void setShaderId(GLuint* shader);
	// gameObject
	void addGameObject(cGameObject* theGO);
	void removeGameObject(cGameObject* theGO);
	// light
	void addLight(cLight* theGO);
	void removeLight(cLight* theGO);
	// camera
	void addCameraPtr(cFlyCamera* camera);
	// FBO
	void setFBOPtr(cFBO* someFbo);
	cFBO* getFBO();
	// tools to render scene
	void setSceneParams();
	void setCameraParams();
	void drawSceneObjects();
	// final command
	bool update();
};
