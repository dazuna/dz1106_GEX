#include "cScene.h"
#include "../util/tools.h"
#include "../skybox/skybox.h"
#include <utility>

cScene::~cScene() = default;
cScene::cScene(std::string name_,std::string jsonPath_, const std::string& stringEffect_,
				std::vector<int> camIndex_,int sceneWidth_,int sceneHeight_)
	:name(std::move(name_)),jsonPath(std::move(jsonPath_)),stringEffect(stringEffect_),
	effect(convertStrToSceneEffect(stringEffect_)),camIndex(std::move(camIndex_)),
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

void cScene::setShaderId(GLuint* shader)
{
	this->shaderProgramId = shader;
}

void cScene::addGameObject(cGameObject* theGO)
{
	this->sceneGameObjects.insert({theGO->friendlyName,theGO});
}

void cScene::removeGameObject(cGameObject* theGO)
{
	this->sceneGameObjects.erase(theGO->friendlyName);
}

void cScene::addLight(cLight* theGO)
{
	this->sceneLights.insert({theGO->name,theGO});
}

void cScene::removeLight(cLight* theGO)
{
	this->sceneLights.erase(theGO->name);
}

void cScene::addCameraPtr(cFlyCamera* camera)
{
	this->cameras.push_back(camera);
}

void cScene::setFBOPtr(cFBO* someFbo)
{
	this->theFBO = someFbo;
}

cFBO* cScene::getFBO()
{
	return this->theFBO;
}

void cScene::setSceneParams()
{
	// Draw everything to the external frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->theFBO->ID);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->theFBO->clearBuffers(true, true);
	
	// Set the passNumber to sceneNumber
	const GLint passNumber_UniLoc = glGetUniformLocation(*shaderProgramId, "passNumber");
	glUniform1i(passNumber_UniLoc, 0);  //"passNumber"

	glUseProgram(*shaderProgramId);	
}

void cScene::setCameraParams()
{
	int width, height;
	glm::mat4 p, v;

	//glfwGetFramebufferSize(window, &width, &height);
	float ratio = sceneWidth/ (float)sceneHeight;

	// Projection matrix
	p = glm::perspective(0.6f,		// FOV
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		1000.0f);		// Far clipping plane

	// View matrix
	v = glm::mat4(1.0f);

	// v = glm::lookAt(cameraEye,
	// 	cameraTarget,
	// 	upVector);

	v = glm::lookAt( cameras[0]->eye, 
					  cameras[0]->getAtInWorldSpace(), 
					 cameras[0]->getUpVector() );


	GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
	GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");
	
	glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
	glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

	// Clear both the colour buffer (what we see) and the depth (or z) buffer.
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto pLight : ::g_map_pLights)
	{
		pLight.second->setUniforms();
	}

	// Also set the position of my "eye" (the camera)
	//uniform vec4 eyeLocation;
	GLint eyeLocation_UL = glGetUniformLocation(*shaderProgramId, "eyeLocation");

	glUniform4f(eyeLocation_UL,
		cameras[0]->eye.x,
		cameras[0]->eye.y,
		cameras[0]->eye.z, 1.0f);
	
	glViewport(0, 0, sceneWidth, sceneHeight);

	//drawSkyBox();
	pDebugRenderer->RenderDebugObjects(v, p, 0.01f);
	
}

void cScene::drawSceneObjects()
{
	drawSkyBox(cameras[0]->eye);
	for (auto itGO = sceneGameObjects.begin(); itGO != sceneGameObjects.end();itGO++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);
		//if (itGO->second->isVisible)
		//{
		itGO->second->isVisible = true;
		tools::DrawObject(matModel, itGO->second, shaderProgID, pTheVAOManager);
		itGO->second->isVisible = false;
		//}
	}
}

bool cScene::update()
{
	// draw Objects
	setSceneParams();
	setCameraParams();
	drawSceneObjects();
	return true;
}
