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

	if(this->scenesVector.size() == 1)
	{
		loadAllObjectsToScene1();
	}
	
	return result;
}

void cSceneManager::loadAllObjectsToScene1()
{
	for(const auto& theGO : *pGameObjects)
	{
		const auto& actualGO = theGO.second;
		if(actualGO->friendlyName=="stencilator")continue;
		if(actualGO->friendlyName=="defScreen")continue;
		if(actualGO->friendlyName=="theQuad")continue;
		if(actualGO->friendlyName=="cameraPosition0")continue;
		if(actualGO->friendlyName=="cameraTarget0")continue;
		scenesVector[0]->addGameObject(actualGO);
	}
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

void cSceneManager::lastPass(GLFWwindow* window)
{
	// *******
	// Start of 2nd pass
	// The whole scene is now drawn (to the FBO)

	// 1. Disable the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*float ratio;*/
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//ratio = width / (float)height;

	// 2. Clear the ACTUAL screen buffer
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3. Use the FBO colour texture as the texture on that quad
	GLint passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
	glUniform1i(passNumber_UniLoc, 1);  //"passNumber"
	//glUniform1i(passNumber_UniLoc, 0);  //"passNumber"

	// Tie the texture to the texture unit
	glActiveTexture(GL_TEXTURE0 + 40);				// Texture Unit 40!!
	//glBindTexture(GL_TEXTURE_2D, pTheFBO->colourTexture_0_ID);	// Texture now assoc with texture unit 0
	auto texture = this->scenesVector[0]->getFBO();
	//auto texture = this->theStencilScene->getFBO();
	glBindTexture(GL_TEXTURE_2D, texture->colourTexture_0_ID);
//		glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);	// Texture now assoc with texture unit 0
	GLint textSamp00_UL = glGetUniformLocation(shaderProgID, "secondPassColourTexture");
	glUniform1i(textSamp00_UL, 40);	// Texture unit 40

	// 4. Draw a single object (a triangle or quad)
	cGameObject* pQuadOrIsIt = NULL;
	if (tools::pFindObjectByFriendlyNameMap("theQuad"))
	{
		pQuadOrIsIt = ::g_map_GameObjects["theQuad"];
	}
	float oldScale = pQuadOrIsIt->scale;
	pQuadOrIsIt->scale = 50.0f;
	pQuadOrIsIt->isVisible = true;
	pQuadOrIsIt->setOrientation(glm::vec3(0.0f, 180.0f, 0.0f));
	pQuadOrIsIt->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pQuadOrIsIt->isWireframe = false;

	// Move the camera
	// Maybe set it to orthographic, etc.

	glm::mat4 v2 = glm::lookAt(glm::vec3(0.0f, 0.0f, -30.0f),		// Eye
		glm::vec3(0.0f, 0.0f, 0.0f),			// At
		glm::vec3(0.0f, 1.0f, 0.0f));		// Up

	GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
	glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v2));

	// Set the actual screen size
	GLint screenWidth_UnitLoc = glGetUniformLocation(shaderProgID, "screenWidth");
	GLint screenHeight_UnitLoc = glGetUniformLocation(shaderProgID, "screenHeight");

	// Get the "screen" framebuffer size 
	glfwGetFramebufferSize(window, &width, &height);

	glUniform1f(screenWidth_UnitLoc, width);
	glUniform1f(screenHeight_UnitLoc, height);

	glm::mat4 matQuad = glm::mat4(1.0f);
	tools::DrawObject(matQuad, pQuadOrIsIt,
		shaderProgID, pTheVAOManager);

	//drawSkyBox();
	
	//pQuadOrIsIt->scale = oldScale;
	pQuadOrIsIt->isVisible = false;
	// END OF 2nd pass
	// ***********
}

void cSceneManager::drawObjectWithFBO(GLFWwindow* window, std::string name, int sceneNumber)
{
	auto FBO2Draw = scenesVector[sceneNumber]->getFBO();
	int width, height;

	// 3. Use the FBO colour texture as the texture on that quad
	GLint passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
	glUniform1i(passNumber_UniLoc, 50);  //"passNumber"

	// Tie the texture to the texture unit
	glActiveTexture(GL_TEXTURE0 + 45);				// Texture Unit 45!!
	glBindTexture(GL_TEXTURE_2D, FBO2Draw->colourTexture_0_ID);
//		glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);	// Texture now assoc with texture unit 0
	GLint textSamp00_UL = glGetUniformLocation(shaderProgID, "secondPassColourTexture");
	glUniform1i(textSamp00_UL, 45);	// Texture unit 40


	// 4. Draw a single object (a triangle or quad)
	cGameObject* objectWithScene = NULL;
	if (tools::pFindObjectByFriendlyNameMap(name))
	{
		objectWithScene = ::g_map_GameObjects.at(name);
	}
	//objectWithScene->scale = 50.0f;
	objectWithScene->isVisible = true;

	// Move the camera
	// Maybe set it to orthographic, etc.

	//glm::mat4 v2 = glm::lookAt(glm::vec3(0.0f, 0.0f, -30.0f),		// Eye
	//	glm::vec3(0.0f, 0.0f, 0.0f),			// At
	//	glm::vec3(0.0f, 1.0f, 0.0f));		// Up

	//GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
	//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v2));

	//// Set the actual screen size
	//GLint screenWidth_UnitLoc = glGetUniformLocation(shaderProgID, "screenWidth");
	//GLint screenHeight_UnitLoc = glGetUniformLocation(shaderProgID, "screenHeight");

	//// Get the "screen" framebuffer size 
	//glfwGetFramebufferSize(window, &width, &height);

	//glUniform1f(screenWidth_UnitLoc, width);
	//glUniform1f(screenHeight_UnitLoc, height);

	glm::mat4 matQuad = glm::mat4(1.0f);
	tools::DrawObject(matQuad, objectWithScene,
		shaderProgID, pTheVAOManager);

	objectWithScene->isVisible = false;
	// END OF 2nd pass
	// ***********
	
	glUniform1i(passNumber_UniLoc, 0);  //"passNumber"
}

// #################################################### STENCIL STUFF ####################################################
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
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("character2"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("thePlane"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("defScreen"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_1"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_2"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_3"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("wall_4"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("sphereReflect"));
	theStencilScene->preStencilObjs.push_back(pGameObjects->at("sphereRefract"));
}

void cSceneManager::updateStencil(GLFWwindow* window)
{
	theStencilScene->setSceneParams();
	theStencilScene->setCameraParams();

		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// Enable writing to the colour buffer
	glDepthMask(GL_TRUE);								// Enable writing to the depth buffer
	glEnable(GL_DEPTH_TEST);							// Enable depth testing
	glDisable(GL_STENCIL_TEST);							// Disable stencil test

	drawSkyBox(::g_pFlyCamera->eye);
	//drawObjectWithFBO(window,"defScreen",0);
	
	// Draw the room and skybox
	glm::mat4 matModelRoomPass = glm::mat4(1.0f);
	for(auto go : theStencilScene->preStencilObjs)
	{
		go->isVisible = true;
		if(go->friendlyName == "defScreen") continue;
		tools::DrawObject(matModelRoomPass,go,shaderProgID,pTheVAOManager);
		go->isVisible = false;
	}
	//drawObjectWithFBO(window,"defScreen",0);
	
	//// Clear the stencil  (and everything else)
	//glClearStencil(47);			// Buffer will be cleared to 47 (because it's a strange number)		
	//// Clear stencil (to the number 47)
	//glClear(GL_STENCIL_BUFFER_BIT);

	//// I'll keep the depth TEST on
	//// -> Because I want the doorways to overwrite the "back" of the room
	//// 
	//// What do I do with the "write to the depth buffer???"....
	//// 
	//// Set up the "Stencil Operation" 
	//// Buffer is filled with 47s. 
	//// I want to ALWAYS write another number (133)
	////  only where the doorways are.
	//glEnable(GL_STENCIL_TEST);

	//glStencilOp(GL_KEEP,		// Stencil fails KEEP the original value (47)
	//			GL_KEEP,		// Depth fails KEEP the original value
	//			GL_REPLACE);	// Stencil AND depth PASSES, REPLACE with 133

	//glStencilFunc(GL_ALWAYS,	// If is succeed, ALWAYS do this
	//			  133,			// Replace with this
	//			  0xFF );		// Mask of 1111,1111 (no mask)
	//// I don't actually want to see the door stencil mask object
	//// Prevent writing to the colour buffer
	//glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	//// Depth TEST is still active, 
	//// but I don't write to the buffer; 
	//glDepthMask(GL_FALSE);

	//theStencilScene->theStencilObj->isVisible = true;
	//theStencilScene->theStencilObj->disableDepthBufferWrite = true;
	//tools::DrawObject(matModelRoomPass, theStencilScene->theStencilObj, shaderProgID, pTheVAOManager);
	//theStencilScene->theStencilObj->isVisible = false;

	////{
	////	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	////	glDepthMask(GL_TRUE);
	////}
	//
	//// TUrn on writing to the depth buffer
	//glDepthMask(GL_TRUE);
	//// Clear the depth buffer
	//glClear(GL_DEPTH_BUFFER_BIT);

	//// Change the stencil test
	//glStencilOp(GL_KEEP,		// Stencil fails KEEP the original value (47)
	//			GL_KEEP,		// (stencil passes) Depth fails KEEP the original value
	//			GL_KEEP);		// Stencil AND depth PASSES, Keep 133
	//glStencilFunc(GL_EQUAL,		// Test if equal
	//			  133,			//
	//			  0xFF);

	//// Draw the "inside the room" scene...
	//// (only visible where the "door" model drew 133 to the stencil buffer
	//glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	////for (int index = 0; index != pGameObjects->size(); index++)
	//drawSkyBox(::g_pFlyCamera->eye);
	//for (auto index = pGameObjects->begin(); index != pGameObjects->end(); index++)
	//{
	//	cGameObject* pCurrentObject = index->second;
	//	glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix
	//	tools::DrawObject( matModel, pCurrentObject, shaderProgID, pTheVAOManager );

	//}//for (int index...
	//glDisable(GL_STENCIL_TEST);							// Disable stencil test
}
// #################################################### \\STENCIL STUFF ####################################################