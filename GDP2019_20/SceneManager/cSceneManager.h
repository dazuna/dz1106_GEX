#pragma once
#include "cScene.h"
#include <vector>

class cSceneManager
{
	// Singleton !!
	static cSceneManager* theOneAndOnlySceneManager;	
public:
    cSceneManager(/* args */);
    ~cSceneManager();
	static cSceneManager* getTheSceneManager();
	// properties
	std::vector<cScene*> scenesVector;
	// methods
	bool addScene(cScene *pTheScene);
	bool removeScene(cScene* pTheScene);
};

