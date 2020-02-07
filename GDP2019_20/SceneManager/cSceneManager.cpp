#include "cSceneManager.h"

cSceneManager* cSceneManager::theOneAndOnlySceneManager = new cSceneManager();

cSceneManager::cSceneManager() = default;
cSceneManager::~cSceneManager() = default;

cSceneManager* cSceneManager::getTheSceneManager()
{
	return theOneAndOnlySceneManager;
}

bool cSceneManager::addScene(cScene* pTheScene)
{
	this->scenesVector.push_back(pTheScene);
	return true;
}

bool cSceneManager::removeScene(cScene* pTheScene)
{
	for(auto itSC = this->scenesVector.begin();
		itSC != this->scenesVector.end(); itSC++)
	{
		auto scene = *itSC;
		if(scene->name == pTheScene->name)
		{
			scenesVector.erase(itSC);
			return true;
		}
	}
	return false;
}

