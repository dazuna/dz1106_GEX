#pragma once
#include <vector>
#include "cScene.h"
#include "cStencilScene.h"
#include "../cLight.h"

class cSceneManager
{
	// Singleton !!
	static cSceneManager* theOneAndOnlySceneManager;
	
public:	
    ~cSceneManager();
	static cSceneManager* getTheSceneManager();

	// stencil specific stuff
	cStencilScene* theStencilScene;
	void createStencilScene();
	void loadPreStencilBuffer();
	void updateStencil(GLFWwindow* window);

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
	void loadAllObjectsToScene1();
	bool addScene(cScene *pTheScene);
	bool removeScene(cScene* pTheScene);
	void setCameras2Scene(cScene* pTheScene);
	bool setSceneFBO(cScene* pTheScene);
	void setShaderId(GLuint* shader);
	void createCameraArray();
	bool update();
	void lastPass(GLFWwindow* window);
	void drawObjectWithFBO(GLFWwindow* window, std::string name, int sceneNumber);
};

