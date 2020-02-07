#pragma once
#include <string>
#include "../cFBO/cFBO.h"
#include "../cGameObject.h"
#include <map>

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
    cScene(std::string name_,std::string jsonPath_,std::string stringEffect_,
		int cameraNumber_,int sceneWidth_,int sceneHeight_);
	static sceneEffect convertStrToSceneEffect(const std::string& string);
	void addGameObject(cGameObject* theGO);
	void removeGameObject(cGameObject* theGO);
	std::map<std::string,cGameObject*> sceneGameObjects;
	
	// properties
	std::string name;
	std::string jsonPath;
	std::string stringEffect;
	sceneEffect effect;
	int cameraNumber;
	int sceneWidth;
	int sceneHeight;
};
