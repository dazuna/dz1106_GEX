#include "cScene.h"
#include <utility>

cScene::~cScene() = default;
cScene::cScene(std::string name_,std::string jsonPath_,std::string stringEffect_,
				int cameraNumber_,int sceneWidth_,int sceneHeight_)
	:name(std::move(name_)),jsonPath(std::move(jsonPath_)),stringEffect(stringEffect_),
	effect(convertStrToSceneEffect(stringEffect_)),cameraNumber(cameraNumber_),
	sceneWidth(sceneWidth_),sceneHeight(sceneHeight_){}

cScene::sceneEffect cScene::convertStrToSceneEffect(const std::string& string)
{
	if (string == "bloom")
	{
		return cScene::sceneEffect::bloom;
	}
	if (string == "black_white")
	{
		return cScene::sceneEffect::black_white;
	}
	if (string == "night_vision")
	{
		return cScene::sceneEffect::night_vision;
	}
	if (string == "blur")
	{
		return cScene::sceneEffect::blur;
	}
	return cScene::sceneEffect::none;
}

void cScene::addGameObject(cGameObject* theGO)
{
	this->sceneGameObjects.insert({theGO->friendlyName,theGO});
}

void cScene::removeGameObject(cGameObject* theGO)
{
	this->sceneGameObjects.erase(theGO->friendlyName);
}