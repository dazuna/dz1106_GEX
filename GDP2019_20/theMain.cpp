#include "GLCommon.h"

#include "globalStuff.h"
#include "util/tools.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <stdlib.h>
#include <iostream>
#include <map>		
#include <sstream>
#include <float.h>
#include <string>
#include "cModelLoader.h"			
#include "cVAOManager.h"
#include "cGameObject.h"
#include "cShaderManager.h"
#include "TextureManager/cBasicTextureManager.h"
#include "JSONLoader.h"// JSON Stuff	
#include "cPhysics.h"
#include "cLowPassFilter.h"
//#include "PhysicsStuff.h"// The Physics function
//#include "cAABB/cAABB.h"
#include "cAABB/PhysicsAABBStuff.h"
#include "DebugRenderer/cDebugRenderer.h"	
#include "cLight.h"// Used to visualize the attenuation of the lights...
#include "cFlyCamera/cFlyCamera.h"
#include "skybox/skybox.h"
#include "GFLW_callbacks.h"// Keyboard, error, mouse, etc. are now here
#include "cFBO/cFBO.h"
#include "cAnimatedPlayer/cAnimatedPlayer.h"

cFBO* pTheFBO = NULL;

cFlyCamera* g_pFlyCamera = NULL;
cGameObject* pSkyBox = new cGameObject();
glm::vec3 cameraEye = glm::vec3(0.0f, 50.0f, 100.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 50.0f, 0.0f);
glm::vec3 visionVector = glm::normalize(cameraTarget - cameraEye);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
// vec3(21.258450, 22.228125, 37.885464)

// This is a "normalized" direction (i.e. the length is 1.0f)
glm::vec3 sexyLightSpotDirection = glm::vec3(0.0f, -1.0f, 0.0f);

bool everythingWireFrame = false;
bool debugger = false;
std::string console;
GLuint shaderProgID;
cVAOManager* pTheVAOManager = new cVAOManager();
cModelLoader* pTheModelLoader = new cModelLoader();
cDebugRenderer* pDebugRenderer = new cDebugRenderer();
cPhysics* pPhysic = new cPhysics();
cBasicTextureManager* pTextureManager = NULL;
extern std::map<unsigned long long /*ID*/, cAABB*> g_mapAABBs_World;
playerController* pPlayerControl;
//cLuaBrain* p_LuaScripts;
//extern std::map<unsigned long long /*ID*/, cAABB*> g_vecAABBs_World;

// pirateStuff
double timer = 0.0;
double averageDeltaTime;
bool isDroneOn = false;
bool cameraFollowPlayer = false;
int width, height;

// cool effects
bool isBloom = false;
bool isNightVision = false;

// Load up my "scene" objects (now global)
std::map<std::string, cMesh*> g_map_Mesh;
std::map<std::string, cGameObject*> g_map_GameObjects;
std::map<float, cGameObject*> closestTransparentObjects;
std::map<std::string, cGameObject*>::iterator selectedGameObject = g_map_GameObjects.begin();
std::map<std::string, cLight*> g_map_pLights;
std::map<std::string, cLight*>::iterator selectedLight = g_map_pLights.begin();
cSceneManager* theSceneManager = cSceneManager::getTheSceneManager();
cAnimatedPlayer* theAnimatedPlayer = cAnimatedPlayer::getAnimatedPlayer();
selectedType cursorType = selectedType::GAMEOBJECT;

// weirdly specific functions
void rotateTieFighter();
void fluctuateReflectionSphere();

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1280, 720, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	// glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	// Mouse callbacks
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	void ProcessAsyncMouse(GLFWwindow * window);
	void ProcessAsyncKeys(GLFWwindow * window);	

	//	OpenGL and GLFW are good to go, so load the model
	// cModelLoader* pTheModelLoader = new cModelLoader();

	cShaderManager* pTheShaderManager = new cShaderManager();
	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";
	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";
	if (!pTheShaderManager->createProgramFromFile("SimpleShader", vertexShad, fragShader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << pTheShaderManager->getLastError();
		return -1;
	}
	shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");

	// Create a VAO Manager...
	// cVAOManager* pTheVAOManager = new cVAOManager();

	::pTextureManager = new cBasicTextureManager();
	
	// SkyBoxTexture
	setSkyBoxTexture();

	::pTextureManager->SetBasePath("assets/textures");

	//JSON Loader for objects
	JSONLoader::JSONLoadGameObjects(&::g_map_GameObjects);
	JSONLoader::JSONLoadMeshes(&g_map_Mesh, pTheModelLoader);
	JSONLoader::JSONLoadTextures(&::g_map_GameObjects, ::pTextureManager);
	JSONLoader::loadMeshToGPU(pTheVAOManager, &::g_map_Mesh, &::g_map_GameObjects, shaderProgID);
	selectedGameObject = ::g_map_GameObjects.begin();

	//	//	mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	//cPhysics* pPhysic = new cPhysics();
	cLowPassFilter avgDeltaTimeThingy;

	// Let there be lights.. I guess
	JSONLoader::JSONLoadLights(&::g_map_pLights,shaderProgID);
	selectedLight = ::g_map_pLights.begin();

	//::theSceneManager->init();

	if(g_map_GameObjects.find("character") != g_map_GameObjects.end())
	{
		::theAnimatedPlayer->addPlayableObject(g_map_GameObjects.at("character"));
		::theAnimatedPlayer->selectedPlayable = ::theAnimatedPlayer->playAnimChars.begin();
	}

	if(g_map_GameObjects.find("character2") != g_map_GameObjects.end())
	{
		::theAnimatedPlayer->addPlayableObject(g_map_GameObjects.at("character2"));
		::theAnimatedPlayer->selectedPlayable = ::theAnimatedPlayer->playAnimChars.begin();
	}

	// Adjust camera to first position (if existent in map)
	if (tools::pFindObjectByFriendlyNameMap("cameraPosition0"))
	{
		cameraEye = ::g_map_GameObjects["cameraPosition0"]->positionXYZ;
		if (tools::pFindObjectByFriendlyNameMap("cameraTarget0"))
		{
			cameraTarget = ::g_map_GameObjects["cameraTarget0"]->positionXYZ;
			visionVector = glm::normalize(cameraTarget - cameraEye);
		}
		cameraTarget = cameraEye + visionVector;
	}

	::g_pFlyCamera = new cFlyCamera();
	::g_pFlyCamera->eye = cameraEye;
	::g_pFlyCamera->cameraLookAt(cameraTarget);
	::g_pFlyCamera->movementSpeed = 100.0f;

	::theSceneManager->init();
	::theSceneManager->createStencilScene();
	
	// todo: generar nuevos objetos eye y target para cada escena y crear la camara viendo a ellos.

	// Get the initial time
	double lastTime = glfwGetTime();
	std::cout << "start loop!" << std::endl;

	createSkyBoxObject();

	// Set up the FBO object
	pTheFBO = new cFBO();
	// Usually we make this the size of the screen.
	std::string FBOError;
	if (pTheFBO->init(1280, 720, FBOError))
	{
		std::cout << "Frame buffer is OK" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
	}

	pDebugRenderer->initialize();
	
	while (!glfwWindowShouldClose(window))
	{
		//// Draw everything to the external frame buffer
		//// (I get the frame buffer ID, and use that)
		//glBindFramebuffer(GL_FRAMEBUFFER, pTheFBO->ID);
		//pTheFBO->clearBuffers(true, true);
		////// Set the passNumber to 0
		//GLint passNumber_UniLoc = glGetUniformLocation(shaderProgID, "passNumber");
		//glUniform1i(passNumber_UniLoc, 0);  //"passNumber"

		// Get the initial time
		double currentTime = glfwGetTime();
		// Frame time... (how many seconds since last frame)
		double deltaTime = currentTime - lastTime;
		timer += currentTime - lastTime;
		lastTime = currentTime;

		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME) { deltaTime = SOME_HUGE_TIME; }
		avgDeltaTimeThingy.addValue(deltaTime);

		ProcessAsyncKeys(window);
		ProcessAsyncMouse(window);

		glUseProgram(shaderProgID);
		
		//float ratio;
		//int width, height;
		//glm::mat4 p, v;
		//
		//glfwGetFramebufferSize(window, &width, &height);
		//ratio = width / (float)height;
		//
		//// Projection matrix
		//p = glm::perspective(0.6f,		// FOV
		//	ratio,			// Aspect ratio
		//	0.1f,			// Near clipping plane
		//	15000.0f);		// Far clipping plane
		//
		//// View matrix
		//v = glm::mat4(1.0f);
		//
		//// v = glm::lookAt(cameraEye,
		//// 	cameraTarget,
		//// 	upVector);
		//
		//v = glm::lookAt( ::g_pFlyCamera->eye, 
		//				 ::g_pFlyCamera->getAtInWorldSpace(), 
		//				 ::g_pFlyCamera->getUpVector() );
		//
		//glViewport(0, 0, width, height);
		//
		//// Clear both the colour buffer (what we see) and the depth (or z) buffer.
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		//////----------------------------
		//////		AQUI IBA LIGHTS
		////// ---------------------------
		//for (auto pLight : ::g_map_pLights)
		//{
		//	pLight.second->setUniforms();
		//}
		//
		//// Also set the position of my "eye" (the camera)
		////uniform vec4 eyeLocation;
		//GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");
		//
		//glUniform4f(eyeLocation_UL,
		//	::g_pFlyCamera->eye.x,
		//	::g_pFlyCamera->eye.y,
		//	::g_pFlyCamera->eye.z, 1.0f);
		
		std::stringstream ssTitle;
		tools::setWindowTitle(&ssTitle);
		glfwSetWindowTitle(window, ssTitle.str().c_str());
		
		//GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		//GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");
		//
		//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		//glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));
		//
		//drawSkyBox();
		//
		//// ************************** order transparent objects **************************
		//tools::makeTransparentObjectsMap();
		//std::vector<cGameObject*> theWorldVector = tools::getWorldMapAsVector();
		//
		//// **************************************************
		//// Loop to draw everything in the scene
		//for (int index = 0; index < theWorldVector.size(); index++)
		//{
		//	glm::mat4 matModel = glm::mat4(1.0f);
		//	if (theWorldVector[index]->isVisible)
		//	{
		//		tools::DrawObject(matModel, theWorldVector[index], shaderProgID, pTheVAOManager);
		//	}
		//}//for (int index...

		
		//::theSceneManager->drawObjectWithFBO(window,"defScreen",0);
		rotateTieFighter();
		fluctuateReflectionSphere();
		theSceneManager->update();
		theSceneManager->updateStencil(window);
		// TODO: Invert the rule for pre Stencil Buffer...
		
		switch (cursorType)
		{
		case selectedType::GAMEOBJECT:tools::drawGameObjectXYZ(pDebugRenderer); break;
		case selectedType::LIGHT:tools::drawLightXYZ(pDebugRenderer);break;
		case selectedType::SOUND:break;
		}

		//	Update the objects through physics
		averageDeltaTime = avgDeltaTimeThingy.getAverage();
		IntegrationStep_AAB(::g_map_GameObjects,float(averageDeltaTime));
		pPhysic->TestForCollisions(::g_map_GameObjects);		

		::theSceneManager->lastPass(window);
		
		glm::mat4 p, v; float ratio;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / float(height);
		p = glm::perspective(0.6f,ratio,0.1f,15000.0f);
		v = glm::mat4(1.0f);
		v = glm::lookAt( ::g_pFlyCamera->eye,::g_pFlyCamera->getAtInWorldSpace(),::g_pFlyCamera->getUpVector() );
		//::theSceneManager->drawObjectWithFBO(window,"defScreen",0);
		pDebugRenderer->RenderDebugObjects(v, p, 0.01f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete everything
	delete pTheModelLoader;
	// delete pTheVAOManager;

	exit(EXIT_SUCCESS);
}

void rotateTieFighter()
{
	if(::g_map_GameObjects.find("tieFighter") != ::g_map_GameObjects.end())
	{
		auto tie = ::g_map_GameObjects.at("tieFighter");
		tie->updateOrientation(glm::vec3(0,1,0));
	}
}

void fluctuateReflectionSphere()
{
	if(::g_map_GameObjects.find("sphereReflect") != ::g_map_GameObjects.end())
	{
		auto refSphere = ::g_map_GameObjects.at("sphereReflect");
		refSphere->scale += float(::averageDeltaTime);
		if(refSphere->scale > 20.f)
		{ refSphere->scale = 15.f; }
	}
}