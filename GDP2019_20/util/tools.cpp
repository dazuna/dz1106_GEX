#include "tools.h"

static glm::vec3 white = glm::vec3(1, 1, 1);
static glm::vec3 red = glm::vec3(1, 0, 0);
static glm::vec3 green = glm::vec3(0, 1, 0);
static glm::vec3 blue = glm::vec3(0, 0, 1);
static glm::vec3 cyan = glm::vec3(0, 1, 1);
static glm::vec3 yellow = glm::vec3(1, 1, 0);


glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject)
{
	glm::mat4 matWorld = glm::mat4(1.0f);
	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans;
	if(pCurrentObject->physicsComponent)
	{
		pCurrentObject->physicsComponent->GetTransform(matTrans);
	}
	else
	{
		matTrans = glm::translate(glm::mat4(1.0f),
								  glm::vec3(pCurrentObject->positionXYZ.x,
									pCurrentObject->positionXYZ.y,
									pCurrentObject->positionXYZ.z));
	}
	
	matWorld = matWorld * matTrans;
	// ******* TRANSLATION TRANSFORM *********

	// ******* ROTATION TRANSFORM *********
	glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
	matWorld = matWorld * matRotation;
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
	// Turns on "alpha transparency"
	// Reads what's on the buffer already, and blends it (RGB+A)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// texture bindings
	SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);
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
	// glUniform4f(diffuseColour_UL,
	// 	pCurrentObject->objectColourRGBA.r,
	// 	pCurrentObject->objectColourRGBA.g,
	// 	pCurrentObject->objectColourRGBA.b,
	// 	pCurrentObject->objectColourRGBA.a);
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->alphaTransparency);


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
			pCurrentObject->objectColourRGBA.r,
			pCurrentObject->objectColourRGBA.g,
			pCurrentObject->objectColourRGBA.b,
			pCurrentObject->objectColourRGBA.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
		glCullFace(GL_BACK);
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

	if (pCurrentObject->friendlyName == "wallBack")
	{
		GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "makeHoles");
		glUniform1f(bMakeHoles_UL, (float)GL_TRUE);
	}
	else
	{
		GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "makeHoles");
		glUniform1f(bMakeHoles_UL, (float)GL_FALSE);
	}

	if (debugger)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (itsDeadJim && pCurrentObject->friendlyName == "ztarDestroyer")
	{
		GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "itsDeadJim");
		glUniform1f(bMakeHoles_UL, (float)GL_TRUE);
		GLint boffset_UL = glGetUniformLocation(shaderProgID, "offset");
		glUniform1f(boffset_UL, ::offset);
	}
	else
	{
		GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "itsDeadJim");
		glUniform1f(bMakeHoles_UL, (float)GL_FALSE);
	}

	// ************************** SKYBOX **************************
	// glCullFace(GL_BACK) only front facing tris are drawn. --> EVERYTHING ELSE IS DISABLED
	// make a draw skybox subfunction... :D
	// disable everything and load the skybox first

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
bool pFindObjectByFriendlyNameMap(std::string name)
{
	//std::map<std::string, cGameObject*> g_map_GameObjectsByFriendlyName;
	std::map<std::string, cGameObject*>::iterator itGO = ::g_map_GameObjects.find(name);
	if (itGO != ::g_map_GameObjects.end())
		return true;
	else
		return false;
}

void drawLightXYZ(cDebugRenderer* pDebugRenderer)
{
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(2.0f, 0.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(0.0f, 2.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ,
		(selectedLight->second.positionXYZ + glm::vec3(0.0f, 0.0f, 2.0f)),
		white);
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		selectedLight->second.positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	// z triangle
	pDebugRenderer->addTriangle(
		selectedLight->second.positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	// square
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second.positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second.positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	if (selectedLight->second.type == 1.0f)
	{
		glm::vec3 spotNormal = selectedLight->second.getCurrentAT();
		spotNormal = glm::normalize(spotNormal);
		pDebugRenderer->addLine(
			selectedLight->second.positionXYZ,
			selectedLight->second.positionXYZ + (spotNormal*2.0f),
			yellow);
	}
}

void drawGameObjectXYZ(cDebugRenderer* pDebugRenderer)
{
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(2.0f, 0.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 2.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 0.0f, 2.0f)),
		white);
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	// z triangle
	pDebugRenderer->addTriangle(
		selectedGameObject->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	// square
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedGameObject->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	// AT & UP
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + (selectedGameObject->second->getCurrentAT()*3.0f)),
		cyan);
	pDebugRenderer->addLine(
		selectedGameObject->second->positionXYZ,
		(selectedGameObject->second->positionXYZ + (selectedGameObject->second->getCurrentUP() * 3.0f)),
		yellow);
}

void drawNormalsXYZ(cDebugRenderer* pDebugRenderer)
{
	//selectedGameObject->second;
	cMesh transMesh;
	glm::mat4 matWorld = glm::mat4(1.0f);
	CalculateTransformedMesh(*::g_map_Mesh[selectedGameObject->second->meshName.c_str()], matWorld, transMesh);
	//std::vector<sPlyVertexXYZ_N_UV> vecVertices;
	std::vector<sPlyVertexXYZ_N_UV>::iterator itVerts;
	for (itVerts = transMesh.vecVertices.begin(); itVerts != transMesh.vecVertices.end(); itVerts++)
	{
		pDebugRenderer->addLine(
			glm::vec3(itVerts->x, itVerts->y, itVerts->z),
			glm::vec3(itVerts->nx, itVerts->ny, itVerts->nz),
			glm::vec3(1.0f, 1.0f, 1.0f));
	}
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
	case selectedType::GAMEOBJECT:
		*ssTitle << " object: " << selectedGameObject->first.c_str()
			<< " posXYZ: " << GLMvec3toString(selectedGameObject->second->positionXYZ)
			<< " colRGB: " << GLMvec3toString(selectedGameObject->second->objectColourRGBA);
		break;
	case selectedType::LIGHT:
		*ssTitle << " light: " << selectedLight->first.c_str()
			<< " posXYZ: " << GLMvec3toString(selectedLight->second.positionXYZ)
			<< " linearA: " << selectedLight->second.LinearAtten
			<< " quadA: " << selectedLight->second.QuadraticAtten
			<< " inner: " << selectedLight->second.innerAngle
			<< " outer: " << selectedLight->second.outerAngle;
		break;
	case selectedType::SOUND:break;
	}
	*ssTitle << " || FPS: " << (int)(1 / averageDeltaTime);
	*ssTitle << " || leftShield: " << leftShieldHealth;
	*ssTitle << " || rightShield: " << rightShieldHealth;
	if (itsDeadJim)
	{
		*ssTitle << "You've destroyed the Star Destroyer!";
	}
	//*ssTitle << " isDroneOn: " << isDroneOn;
	//*ssTitle << "   Tgt: " << GLMvec3toString(cameraTarget);
	//*ssTitle << "   Vis: " << GLMvec3toString(visionVector);
	//*ssTitle << "   XYZ: " << GLMvec3toString(visionVector);
}

void drawPyramidPlayer(cDebugRenderer* pDebugRenderer)
{
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// z triangle
	pDebugRenderer->addTriangle(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	// square
	pDebugRenderer->addLine(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	pDebugRenderer->addLine(
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		::g_map_GameObjects["spherePlayer"]->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

void makeSkullEyesFlicker()
{
	//std::map<std::string, cLight> g_map_pLights;
	std::map<std::string, cLight>::iterator itLite;

	if (timer >= 0.1)
	{
		timer = 0.0;
		itLite = ::g_map_pLights.find("candleLight1");
		if (itLite != g_map_pLights.end())
		{
			itLite->second.QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		itLite = ::g_map_pLights.find("candleLight2");
		if (itLite != g_map_pLights.end())
		{
			itLite->second.QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		itLite = ::g_map_pLights.find("candleLight3");
		if (itLite != g_map_pLights.end())
		{
			itLite->second.QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		makeCameraDroneAround(isDroneOn);
	}
}

void makeCameraDroneAround(bool isDroneOn)
{
	float degreesOfRotation = 0.5f;
	// printf("makeCameraDroneAround");
	if (pFindObjectByFriendlyNameMap("cameraPosition0") && isDroneOn)
	{
		glm::mat4 matRotY = glm::rotate(glm::mat4(1.0f), glm::radians(degreesOfRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 objectVector = ::g_map_GameObjects["cameraPosition0"]->positionXYZ - ::g_map_GameObjects["cameraTarget0"]->positionXYZ;
		objectVector = glm::vec3(matRotY * glm::vec4(objectVector, 1.0));
		::g_map_GameObjects["cameraPosition0"]->positionXYZ = ::g_map_GameObjects["cameraTarget0"]->positionXYZ + objectVector;

		cameraEye = ::g_map_GameObjects["cameraPosition0"]->positionXYZ;
		if (pFindObjectByFriendlyNameMap("cameraTarget0"))
		{
			cameraTarget = ::g_map_GameObjects["cameraTarget0"]->positionXYZ;
			visionVector = glm::normalize(cameraTarget - cameraEye);
		}
		cameraTarget = cameraEye + visionVector;
	}
}

std::vector<cGameObject*> getWorldMapAsVector()
{
	std::vector<cGameObject*> tempVec;
	std::vector<cGameObject*> tempVec2;
	std::map<std::string, cGameObject*>::iterator itGO;	
	if (::closestTransparentObjects.size() >= 1)
	{
		// vector1.insert(vector1.end(), vector2.begin(), vector2.end());
		std::map<float, cGameObject*>::iterator itGO2;
		for (itGO = ::g_map_GameObjects.begin(); itGO != ::g_map_GameObjects.end(); itGO++)
		{
			if (itGO->second->alphaTransparency == 1)
			{
				tempVec.push_back(itGO->second);
			}				
		}
		tempVec2 = getClosestTransparentObjectsAsVector();
		tempVec.insert(tempVec.end(), tempVec2.begin(), tempVec2.end());
	}
	else
	{
		for (itGO = ::g_map_GameObjects.begin(); itGO != ::g_map_GameObjects.end(); itGO++)
		{
			tempVec.push_back(itGO->second);
		}
	}
	return tempVec;
}

std::vector<cGameObject*> getClosestTransparentObjectsAsVector()
{
	std::vector<cGameObject*> tempVec;
	std::map<float, cGameObject*>::reverse_iterator it;
	for (it = ::closestTransparentObjects.rbegin();
		it != ::closestTransparentObjects.rend(); it++) 
	{
		tempVec.push_back(it->second);
	}
	return tempVec;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& input)
{
	for (auto const& i : input) {
		os << i << " ";
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<float>& input)
{
	for (auto const& i : input) {
		os << i << " ";
	}
	return os;
}
void printGameObject(cGameObject* theGO)
{
	std::cout << "friendlyName:\t" << theGO->friendlyName << std::endl;			
	std::cout << "meshName:\t" << theGO->meshName << std::endl;			
	std::cout << "meshURL:\t" << theGO->meshURL << std::endl;			
	std::cout << "positionXYZ:\t" << glm::to_string(theGO->positionXYZ) << std::endl;			
	//std::cout << "rotationXYZ:\t" << glm::to_string(theGO->rotationXYZ) << std::endl;			
	std::cout << "objectColourRGBA:\t" << glm::to_string(theGO->objectColourRGBA) << std::endl;			
	std::cout << "alphaTransparency:\t" << theGO->alphaTransparency << std::endl;			
	std::cout << "diffuseColour:\t" << glm::to_string(theGO->diffuseColour) << std::endl;			
	std::cout << "specularColour:\t" << glm::to_string(theGO->specularColour) << std::endl;			
	std::cout << "scale:\t" << theGO->scale << std::endl;			
	//std::cout << "matWorld:\t" << glm::to_string(theGO->matWorld) << std::endl;			
	std::cout << "velocity:\t" << glm::to_string(theGO->velocity) << std::endl;			
	std::cout << "accel:\t" << glm::to_string(theGO->accel) << std::endl;			
	std::cout << "textures:\t" << theGO->textures << std::endl;			
	std::cout << "textureRatio:\t" << theGO->textureRatio << std::endl;			
	std::cout << "tag:\t" << theGO->tag << std::endl;			
	std::cout << "lifetime:\t" << theGO->lifetime << std::endl;			
	std::cout << "inverseMass:\t" << theGO->inverseMass << std::endl;			
	std::cout << "physicsShapeType:\t" << theGO->physicsShapeType << std::endl;			
	//std::cout << "AABB_min:\t" << glm::to_string(theGO->AABB_min) << std::endl;			
	//std::cout << "AABB_max:\t" << glm::to_string(theGO->AABB_max) << std::endl;			
	std::cout << "SPHERE_radius:\t" << theGO->SPHERE_radius << std::endl;			
	std::cout << "isWireframe:\t" << theGO->isWireframe << std::endl;			
	std::cout << "debugColour:\t" << glm::to_string(theGO->debugColour) << std::endl;			
	std::cout << "isVisible:\t" << theGO->isVisible << std::endl;			
	std::cout << "disableDepthBufferTest:\t" << theGO->disableDepthBufferTest << std::endl;			
	std::cout << "disableDepthBufferWrite:\t" << theGO->disableDepthBufferWrite << std::endl;			
	std::cout << "" << "" << std::endl;			
}

void makeTransparentObjectsMap()
{
	closestTransparentObjects.clear();
	std::map<std::string, cGameObject*>::iterator itGO;
	for (itGO = ::g_map_GameObjects.begin(); itGO != ::g_map_GameObjects.end(); itGO++)
	{
		if (itGO->second->alphaTransparency < 1.0f)
		{
			float distance = glm::distance(itGO->second->positionXYZ, ::g_pFlyCamera->eye);
			closestTransparentObjects.insert({ distance,itGO->second });
		}
	}
}

// Takes a mesh in "model space" and converts it into "world space"
void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld,cMesh& mesh_transformedInWorld)
{
	mesh_transformedInWorld = originalMesh;
	for (std::vector<sPlyVertexXYZ_N_UV>::iterator itVert = mesh_transformedInWorld.vecVertices.begin();
		itVert != mesh_transformedInWorld.vecVertices.end(); itVert++)
	{
		glm::vec4 vertex = glm::vec4(itVert->x, itVert->y, itVert->z, 1.0f);
		glm::vec4 vertexWorldTransformed = matWorld * vertex;
		// Update 
		itVert->x = vertexWorldTransformed.x;
		itVert->y = vertexWorldTransformed.y;
		itVert->z = vertexWorldTransformed.z;
		// CALCAULTE THE NORMALS for the this mesh, too (for the response)
		// for the normal, do the inverse transpose of the world matrix
		glm::mat4 matWorld_Inv_Transp = glm::inverse(glm::transpose(matWorld));
		glm::vec4 normal = glm::vec4(itVert->nx, itVert->ny, itVert->nz, 1.0f);
		glm::vec4 normalWorldTransformed = matWorld_Inv_Transp * normal;
		// Update 
		itVert->nx = normalWorldTransformed.x;
		itVert->ny = normalWorldTransformed.y;
		itVert->nz = normalWorldTransformed.z;
	}

	return;
}

void drawAABBs()
{
	std::map<unsigned long long, cAABB*>::iterator itAABB;
	for (itAABB = ::g_mapAABBs_World.begin(); itAABB != ::g_mapAABBs_World.end(); itAABB++)
	{
		itAABB->second->drawAABBSelf(::pDebugRenderer);
	}
}

void drawSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime)
{
	cGameObject* pGO = new cGameObject(::g_map_GameObjects["sphere"]);
	pGO->positionXYZ = position;
	if (texture != "")
	{
		pGO->textures[3] = texture;
		pGO->textureRatio[3] = 1.0f;
		pGO->textureRatio[0] = 0.0f;
	}
	pGO->alphaTransparency = alphaT;
	pGO->tag = "lifetime";
	pGO->lifetime = 10.0f;
	pGO->scale = sscale;
	pGO->isVisible = true;
	::g_map_GameObjects.insert({ pGO->friendlyName,pGO });
}

void duplicateSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime)
{
	cGameObject* pGO = new cGameObject(::g_map_GameObjects["sphere"]);
	if (texture == "red")
	{
		pGO = new cGameObject(::g_map_GameObjects["sphereRed"]);
	}
	if (texture == "yellow")
	{
		pGO = new cGameObject(::g_map_GameObjects["sphereYellow"]);
	}
	if (texture == "white")
	{
		pGO = new cGameObject(::g_map_GameObjects["sphereWhite"]);
	}
	pGO->positionXYZ = position;
	pGO->alphaTransparency = alphaT;
	pGO->tag = "lifetime";
	pGO->lifetime = lifetime;
	pGO->scale = sscale;
	pGO->isVisible = true;
	::g_map_GameObjects.insert({ pGO->friendlyName,pGO });
}

void lifetimeValidation(cGameObject* pCurObject)
{
	if (pCurObject->tag == "lifetime")
	{
		if (pCurObject->lifetime < 1.0f)
		{
			::g_map_GameObjects.erase(pCurObject->friendlyName.c_str());
		}
		pCurObject->lifetime -= 1.0f;
	}
}
