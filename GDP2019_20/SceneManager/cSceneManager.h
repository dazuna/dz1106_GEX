#pragma once
#include <vector>
#include "cScene.h"
#include "../cLight.h"

class cSceneManager
{
	// Singleton !!
	static cSceneManager* theOneAndOnlySceneManager;
	
public:	
    ~cSceneManager();
	static cSceneManager* getTheSceneManager();
	
	// properties
	std::map<std::string, cGameObject*>* pGameObjects;
	std::map<std::string, cLight*>* pLights;	
	std::vector<cScene*> scenesVector;
	std::vector<cFlyCamera*> gCameras;
	GLuint* shaderProgramId;
	int numScenes;
	int numCameras;
	
	// methods
	bool init();
	bool addScene(cScene *pTheScene);
	bool removeScene(cScene* pTheScene);
	void setCameras2Scene(cScene* pTheScene);
	bool setSceneFBO(cScene* pTheScene);
	void setShaderId(GLuint* shader);
	void createCameraArray();
	bool update();
};

