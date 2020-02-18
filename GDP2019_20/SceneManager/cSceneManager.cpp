#include "cSceneManager.h"
#include "../JSONLoader.h"

cSceneManager* cSceneManager::theOneAndOnlySceneManager = new cSceneManager();
cSceneManager::~cSceneManager() = default;

cSceneManager* cSceneManager::getTheSceneManager()
{
	return theOneAndOnlySceneManager;
}

bool cSceneManager::init()
{
	bool result = true;
	
	// set pointers to globals
	this->pGameObjects = &::g_map_GameObjects;
	this->pLights = &::g_map_pLights;
	
	// load global config
	result = JSONLoader::JSONLoadSceneConf();

	this->shaderProgramId = &::shaderProgID;

	// load cameras from config
	this->createCameraArray();
	
	for (auto scene : this->scenesVector)
	{
		result = JSONLoader::JSONLoadEntitiesToScene(pGameObjects,pLights,scene);
		if(!result) return false;
		result = setSceneFBO(scene);
		if(!result) return false;
		scene->setShaderId(shaderProgramId);
		this->setCameras2Scene(scene);
	}
	return result;
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

void cSceneManager::setCameras2Scene(cScene* pTheScene)
{
	for(auto index : pTheScene->camIndex)
	{
		pTheScene->addCameraPtr(gCameras[index]);
	}
}

bool cSceneManager::setSceneFBO(cScene* pTheScene)
{
	// Set up the FBO object
	auto fbo = new cFBO();
	
	// Usually we make this the size of the screen.
	std::string FBOError;
	if (fbo->init(pTheScene->sceneWidth, pTheScene->sceneHeight, FBOError))
	{
		std::cout << "Frame buffer is OK" << std::endl;
		// set the fbo to the scene
		pTheScene->setFBOPtr(fbo);
		return true;
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
		return false;
	}	
}

void cSceneManager::setShaderId(GLuint* shader)
{
	this->shaderProgramId = shader;
}

void cSceneManager::createCameraArray()
{
	int i = 0;
	for(i; i<this->numCameras; i++)
	{
		auto pCamera = new cFlyCamera();
		this->gCameras.push_back(pCamera);
	}
	std::cout << "successfully created " << i << " camera(s)." << std::endl;
}

bool cSceneManager::update()
{
	auto result = true;
	for(auto scene : scenesVector)
	{
		result = scene->update();
	}
	return result;
}
