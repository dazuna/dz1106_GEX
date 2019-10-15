#include "GLCommon.h"

#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>		
#include <sstream>
#include <limits.h>
#include <float.h>
#include <string>

#include "cModelLoader.h"			
#include "cVAOManager.h"		// NEW
#include "cGameObject.h"
#include "cShaderManager.h"

// JSON Stuff
#include "JSONLoader.h"

// The Physics function
#include "PhysicsStuff.h"
#include "cPhysics.h"
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"

// Used to visualize the attenuation of the lights...
#include "cLight.h"
#include "LightManager/cLightHelper.h"
// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

void drawLightXYZ(cDebugRenderer* pDebugRenderer);
void drawGameObjectXYZ(cDebugRenderer* pDebugRenderer);
void setWindowTitle(std::stringstream* ssTitle);
std::string GLMvec3toString(glm::vec3 theGLMvec3);
glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
void DrawObject(glm::mat4 m,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager);

glm::vec3 cameraEye = glm::vec3(0.0f, 30.0f, 100.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 visionVector = glm::normalize(cameraTarget - cameraEye);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

float sexyLightSpotInnerAngle = 5.0f;
float sexyLightSpotOuterAngle = 7.5f;
// This is a "normalized" direction (i.e. the length is 1.0f)
glm::vec3 sexyLightSpotDirection = glm::vec3(0.0f, -1.0f, 0.0f);

//bool bLightDebugSheresOn = false;
bool bLightDebugSheresOn = true;
std::string console;

// Load up my "scene" objects (now global)
std::map<std::string, cMesh*> g_map_Mesh;
std::map<std::string, cGameObject*> g_map_GameObjects;
std::map<std::string, cGameObject*>::iterator selectedGameObject = g_map_GameObjects.begin();
std::map<std::string, cLight> g_map_pLights;
std::map<std::string, cLight>::iterator selectedLight = g_map_pLights.begin();
//bool g_BallCollided = false;

selectedType cursorType = selectedType::GAMEOBJECT;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//// Move the sphere to where the camera is and shoot the ball from there...
	//cGameObject* pTheBall = pFindObjectByFriendlyNameMap("sphere");

	//// What's the velocity
	//// Target - eye = direction
	//glm::vec3 direction = glm::normalize(cameraTarget - cameraEye);

	//float speed = 10.0f;

	//pTheBall->velocity = direction * speed;
	//pTheBall->positionXYZ = cameraEye;

	return;
}

// Make a class with a vector of doubles. 
// Set this vector to all zeros. 
// Add a method: addTime();
// Add a method: getAgerage();

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

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();

	//	OpenGL and GLFW are good to go, so load the model
	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

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
	GLuint shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");

	// Create a VAO Manager...
	cVAOManager* pTheVAOManager = new cVAOManager();

	//JSON Loader for objects
	JSONLoadMeshes(&g_map_Mesh, pTheModelLoader);
	JSONLoadGameObjects(&::g_map_GameObjects);
	loadMeshToGPU(pTheVAOManager, &::g_map_Mesh, &::g_map_GameObjects, shaderProgID);
	selectedGameObject = ::g_map_GameObjects.begin();

	//	//	mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	cPhysics* pPhsyics = new cPhysics();
	cLowPassFilter avgDeltaTimeThingy;

	// Let there be lights.. I guess
	cLightHelper* pLightHelper = new cLightHelper();
	JSONLoadLights(&::g_map_pLights,shaderProgID);
	selectedLight = ::g_map_pLights.begin();

	// Get the initial time
	double lastTime = glfwGetTime();
	std::cout << "start loop!" << std::endl;

	while (!glfwWindowShouldClose(window))
	{

		// Get the initial time
		double currentTime = glfwGetTime();
		// Frame time... (how many seconds since last frame)
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME) { deltaTime = SOME_HUGE_TIME; }
		avgDeltaTimeThingy.addValue(deltaTime);

		glUseProgram(shaderProgID);

		float ratio;
		int width, height;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			1000.0f);		// Far clipping plane

		// View matrix
		v = glm::mat4(1.0f);

		v = glm::lookAt(cameraEye,
			cameraTarget,
			upVector);

		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the depth (or z) buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (std::map<std::string, cLight>::iterator itLight = ::g_map_pLights.begin();
													itLight != ::g_map_pLights.end();
													itLight++)
		{
			itLight->second.setUniforms();
		}

		// Also set the position of my "eye" (the camera)
		//uniform vec4 eyeLocation;
		GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			cameraEye.x, cameraEye.y, cameraEye.z, 1.0f);

		std::stringstream ssTitle;
		setWindowTitle(&ssTitle);
		glfwSetWindowTitle(window, ssTitle.str().c_str());

		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		// **************************************************
		// **************************************************
		// Loop to draw everything in the scene
		for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects.begin();
														itGO != ::g_map_GameObjects.end();
														itGO++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			DrawObject(matModel, itGO->second,
				shaderProgID, pTheVAOManager);

		}//for (int index...

		switch (cursorType)
		{
		case selectedType::GAMEOBJECT:drawGameObjectXYZ(pDebugRenderer); break;
		case selectedType::LIGHT:drawLightXYZ(pDebugRenderer);break;
		case selectedType::SOUND:break;
		}
		

		//	Update the objects through physics
		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		pPhsyics->IntegrationStep(::g_map_GameObjects, (float)averageDeltaTime);

		pPhsyics->TestForCollisions(::g_map_GameObjects);
		
		pDebugRenderer->RenderDebugObjects(v, p, 0.01f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete everything
	delete pTheModelLoader;
	//	delete pTheVAOManager;
	//	Watch out!!

	exit(EXIT_SUCCESS);
}

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject)
{
	glm::mat4 matWorld = glm::mat4(1.0f);
	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->positionXYZ.x,
				pCurrentObject->positionXYZ.y,
				pCurrentObject->positionXYZ.z));
	matWorld = matWorld * matTrans;
	// ******* TRANSLATION TRANSFORM *********

	// ******* ROTATION TRANSFORM *********
	// ROTATE Z
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.z,
		glm::vec3(0.0f, 0.0f, 1.0f));
	matWorld = matWorld * rotateZ;
	// ROTATE Y
	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	matWorld = matWorld * rotateY;
	// ROTATE X
	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	matWorld = matWorld * rotateX;
	// ******* ROTATION TRANSFORM *********

	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********
	return matWorld;
}

void DrawObject(glm::mat4 m,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{
	m = calculateWorldMatrix(pCurrentObject);
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));
	//	glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
	//	glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

	//	Calculate the inverse transpose of the model matrix and pass that...
	//	Stripping away scaling and translation, leaving only rotation
	//	Because the normal is only a direction, really
	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b);

	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->objectColourRGBA.a);	// 

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)

	//	uniform vec4 debugColour;
	//	uniform bool bDoNotLight;
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	if (pCurrentObject->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->debugColour.r,
			pCurrentObject->debugColour.g,
			pCurrentObject->debugColour.b,
			pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}
	//glPointSize(15.0f);

	if (pCurrentObject->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}

	sModelDrawInfo drawInfo;
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	return;
} // DrawObject;

// returns NULL (0) if we didn't find it.
cGameObject* pFindObjectByFriendlyNameMap(std::string name)
{
	//std::map<std::string, cGameObject*> g_map_GameObjectsByFriendlyName;
	return ::g_map_GameObjects[name];
}

void drawLightXYZ(cDebugRenderer* pDebugRenderer)
{
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(2.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(0.0f, 2.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(0.0f, 0.0f, 2.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		selectedLight->second.positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// z triangle
	pDebugRenderer->addTriangle(
		selectedLight->second.positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// square
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

void drawGameObjectXYZ(cDebugRenderer* pDebugRenderer)
{
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(2.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 2.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 0.0f, 2.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// z triangle
	pDebugRenderer->addTriangle(
		selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// square
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

std::string GLMvec3toString(glm::vec3 theGLMvec3)
{
	std::stringstream out;
	out << theGLMvec3.x << ", " << theGLMvec3.y << ", " << theGLMvec3.z;
	return out.str();
}

void setWindowTitle(std::stringstream* ssTitle)
{
	switch (cursorType)
	{
	case selectedType::GAMEOBJECT:*ssTitle << " object: " << selectedGameObject->first.c_str()
		<< " posXYZ: " << GLMvec3toString(selectedGameObject->second->positionXYZ); break;
	case selectedType::LIGHT:*ssTitle << " light: " << selectedLight->first.c_str()
		<< " posXYZ: " << GLMvec3toString(selectedLight->second.positionXYZ); break;
	case selectedType::SOUND:break;
	}
	//*ssTitle << "   Eye: " << GLMvec3toString(cameraEye);
	//*ssTitle << "   Tgt: " << GLMvec3toString(cameraTarget);
	//*ssTitle << "   Vis: " << GLMvec3toString(visionVector);
	//*ssTitle << "   XYZ: " << GLMvec3toString(visionVector);
}