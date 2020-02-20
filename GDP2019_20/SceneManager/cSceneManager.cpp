#include "cSceneManager.h"
#include "../JSONLoader.h"
#include "../skybox/skybox.h"

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
	gCameras.push_back(::g_pFlyCamera);
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

void cSceneManager::createStencilScene()
{
	theStencilScene = new cStencilScene();
	
	theStencilScene->sceneWidth = 1280;
	theStencilScene->sceneHeight = 720;

	loadPreStencilBuffer();
	theStencilScene->theStencilObj = pGameObjects->at("stencilator");
	
	auto fbo = new cFBO();
	std::string FBOError;
	if (fbo->init(theStencilScene->sceneWidth, theStencilScene->sceneHeight, FBOError))
	{
		std::cout << "StencilScene Frame buffer is OK" << std::endl; 
		theStencilScene->setFBOPtr(fbo);
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
		return;
	}

	theStencilScene->setShaderId(shaderProgramId);
	theStencilScene->addCameraPtr(::g_pFlyCamera);
	
}

void cSceneManager::loadPreStencilBuffer()
{
	//auto pSO = theStencilScene->preStencilObjs;
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("character"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("thePlane"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_1"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_2"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_3"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_4"));
}

void cSceneManager::updateStencil()
{
	theStencilScene->setSceneParams();
	theStencilScene->setCameraParams();

		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// Enable writing to the colour buffer
	glDepthMask(GL_TRUE);								// Enable writing to the depth buffer
	glEnable(GL_DEPTH_TEST);							// Enable depth testing
	glDisable(GL_STENCIL_TEST);							// Disable stencil test

	drawSkyBox();
	// Draw the room and skybox
	glm::mat4 matModelRoomPass = glm::mat4(1.0f);
	for(auto go : theStencilScene->preStencilObjs)
	{
		go->isVisible = true;
		tools::DrawObject(matModelRoomPass,go,shaderProgID,pTheVAOManager);
		go->isVisible = false;
	}
	
	// Clear the stencil  (and everything else)
	glClearStencil(47);			// Buffer will be cleared to 47 (because it's a strange number)		
	// Clear stencil (to the number 47)
	glClear(GL_STENCIL_BUFFER_BIT);

	// I'll keep the depth TEST on
	// -> Because I want the doorways to overwrite the "back" of the room
	// 
	// What do I do with the "write to the depth buffer???"....
	// 
	// Set up the "Stencil Operation" 
	// Buffer is filled with 47s. 
	// I want to ALWAYS write another number (133)
	//  only where the doorways are.
	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP,		// Stencil fails KEEP the original value (47)
				GL_KEEP,		// Depth fails KEEP the original value
				GL_REPLACE);	// Stencil AND depth PASSES, REPLACE with 133

	glStencilFunc(GL_ALWAYS,	// If is succeed, ALWAYS do this
				  133,			// Replace with this
				  0xFF );		// Mask of 1111,1111 (no mask)
	// I don't actually want to see the door stencil mask object
	// Prevent writing to the colour buffer
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	// Depth TEST is still active, 
	// but I don't write to the buffer; 
	glDepthMask(GL_FALSE);

	theStencilScene->theStencilObj->isVisible = true;
	theStencilScene->theStencilObj->disableDepthBufferWrite = true;
	tools::DrawObject(matModelRoomPass, theStencilScene->theStencilObj, shaderProgID, pTheVAOManager);
	theStencilScene->theStencilObj->isVisible = false;

	//{
	//	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//	glDepthMask(GL_TRUE);
	//}
	
	// TUrn on writing to the depth buffer
	glDepthMask(GL_TRUE);
	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Change the stencil test
	glStencilOp(GL_KEEP,		// Stencil fails KEEP the original value (47)
				GL_KEEP,		// (stencil passes) Depth fails KEEP the original value
				GL_KEEP);		// Stencil AND depth PASSES, Keep 133
	glStencilFunc(GL_EQUAL,		// Test if equal
				  133,			//
				  0xFF);

	// Draw the "inside the room" scene...
	// (only visible where the "door" model drew 133 to the stencil buffer
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//for (int index = 0; index != pGameObjects->size(); index++)
	drawSkyBox();
	for (auto index = pGameObjects->begin(); index != pGameObjects->end(); index++)
	{
		cGameObject* pCurrentObject = index->second;
		glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix
		tools::DrawObject( matModel, pCurrentObject, shaderProgID, pTheVAOManager );

	}//for (int index...
	glDisable(GL_STENCIL_TEST);							// Disable stencil test
}

