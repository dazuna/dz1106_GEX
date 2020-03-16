#include "tools.h"

static glm::vec3 white = glm::vec3(1, 1, 1);
static glm::vec3 red = glm::vec3(1, 0, 0);
static glm::vec3 green = glm::vec3(0, 1, 0);
static glm::vec3 blue = glm::vec3(0, 0, 1);
static glm::vec3 cyan = glm::vec3(0, 1, 1);
static glm::vec3 yellow = glm::vec3(1, 1, 0);
auto HACK_FrameTime = 0.f;


glm::mat4 tools::calculateWorldMatrix(cGameObject* pCurrentObject)
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

void tools::DrawObject(glm::mat4 m,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{
	if(!(pCurrentObject->isVisible)) return;
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
		glDepthMask(GL_FALSE);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
		glDepthMask(GL_TRUE);
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}

	//if (pCurrentObject->friendlyName == "wallBack")
	//{
	//	GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "makeHoles");
	//	glUniform1f(bMakeHoles_UL, (float)GL_TRUE);
	//}
	//else
	//{
	//	GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "makeHoles");
	//	glUniform1f(bMakeHoles_UL, (float)GL_FALSE);
	//}
	//
	//if (debugger)
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//}
	//
	//if (itsDeadJim && pCurrentObject->friendlyName == "ztarDestroyer")
	//{
	//	GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "itsDeadJim");
	//	glUniform1f(bMakeHoles_UL, (float)GL_TRUE);
	//	GLint boffset_UL = glGetUniformLocation(shaderProgID, "offset");
	//	glUniform1f(boffset_UL, ::offset);
	//}
	//else
	//{
	//	GLint bMakeHoles_UL = glGetUniformLocation(shaderProgID, "itsDeadJim");
	//	glUniform1f(bMakeHoles_UL, (float)GL_FALSE);
	//}

	GLint bIsBloom = glGetUniformLocation(shaderProgID, "isBloom");
	glUniform1f(bIsBloom,float(GL_FALSE));
	if (isBloom && pCurrentObject->friendlyName == "defScreen")
	{
		glUniform1f(bIsBloom,float(GL_TRUE));
	}

	GLint bIsNightVision = glGetUniformLocation(shaderProgID, "isNightVision");
	glUniform1f(bIsNightVision,float(GL_FALSE));
	if (isNightVision && pCurrentObject->friendlyName == "theQuad")
	{
		glUniform1f(bIsNightVision,float(GL_TRUE));
	}

	//uniform bool shouldReflect;
	//uniform bool shoulfRefract;	
	GLint bShouldReflect = glGetUniformLocation(shaderProgID, "shouldReflect");
	glUniform1f(bShouldReflect,float(GL_FALSE));
	if (pCurrentObject->friendlyName == "sphereReflect")
	{
		glUniform1f(bShouldReflect,float(GL_TRUE));
	}

	GLint bShouldRefract = glGetUniformLocation(shaderProgID, "shoulfRefract");
	glUniform1f(bShouldRefract,float(GL_FALSE));
	if (pCurrentObject->friendlyName == "sphereRefract")
	{
		glUniform1f(bShouldRefract,float(GL_TRUE));
	}

	// ************************** SKYBOX **************************
	// glCullFace(GL_BACK) only front facing tris are drawn. --> EVERYTHING ELSE IS DISABLED
	// make a draw skybox subfunction... :D
	// disable everything and load the skybox first

	GLint isSkinnedMesh_UniLoc = glad_glGetUniformLocation( shaderProgID, "isSkinnedMesh");


	if (pCurrentObject->pAS != NULL)
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_TRUE);

		// Set to all identity
		const int NUMBEROFBONES = 100;

		// Taken from "Skinned Mesh 2"
		std::vector< glm::mat4x4 > vecFinalTransformation;	
		std::vector< glm::mat4x4 > vecOffsets;
		std::vector< glm::mat4x4 > vecObjectBoneTransformation;
		
		pCurrentObject->pAS->update(float(averageDeltaTime),
			vecFinalTransformation,vecOffsets,vecObjectBoneTransformation);

		// Wait until all threads are done updating.
		HACK_FrameTime += 0.01f;

		{// Forward kinematic stuff
			// "Bone" location is at the origin
			//glm::vec4 boneLocation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			// bone #22 is "B_R_Hand" in this model
			//glm::mat4 matSpecificBone = vecFinalTransformation[22];
			// Transformed into "model" space where that bone is.
			//::g_HACK_vec3_BoneLocationFK = matSpecificBone * boneLocation;
			// If it's in world space
			//::g_HACK_vec3_BoneLocationFK = matModel * ::g_HACK_vec3_BoneLocationFK;
		}// Forward kinematic 

		// Copy all 100 bones to the shader
		GLint matBonesArray_UniLoc = glGetUniformLocation(shaderProgID, "matBonesArray");
		// The "100" is to pass 100 values, starting at the pointer location of matBones[0];
		//glUniformMatrix4fv(matBonesArray_UniLoc, 100, GL_FALSE, glm::value_ptr(matBones[0]));
		GLint numBonesUsed = (GLint)vecFinalTransformation.size();
		glUniformMatrix4fv(matBonesArray_UniLoc, numBonesUsed, 
						   GL_FALSE, 
						   glm::value_ptr(vecFinalTransformation[0]));
	}
	else
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_FALSE);
	}
	// ************************************************

	sModelDrawInfo drawInfo;
	
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,drawInfo.numberOfIndices,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}
	else
	{
		pVAOManager->FindDrawInfoByModelName("sphereMesh", drawInfo);
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,drawInfo.numberOfIndices,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}
	
} // DrawObject;

// returns NULL (0) if we didn't find it.
bool tools::pFindObjectByFriendlyNameMap(std::string name)
{
	//std::map<std::string, cGameObject*> g_map_GameObjectsByFriendlyName;
	const auto itGO = ::g_map_GameObjects.find(name);
	if (itGO != ::g_map_GameObjects.end())
		return true;
	return false;
}

bool tools::pFindLightByNameMap(std::string name)
{
	const auto itGO = ::g_map_pLights.find(name);
	if (itGO != ::g_map_pLights.end())
		return true;
	return false;
}

void tools::drawLightXYZ(cDebugRenderer* pDebugRenderer)
{
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ,
		(selectedLight->second->positionXYZ + glm::vec3(2.0f, 0.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ,
		(selectedLight->second->positionXYZ + glm::vec3(0.0f, 2.0f, 0.0f)),
		white);
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ,
		(selectedLight->second->positionXYZ + glm::vec3(0.0f, 0.0f, 2.0f)),
		white);
	// draw pyramid on top of object
	// x triangle
	pDebugRenderer->addTriangle(
		selectedLight->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	// z triangle
	pDebugRenderer->addTriangle(
		selectedLight->second->positionXYZ + glm::vec3(0.0f, 3.0f, 0.0f),
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	// square
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, 1.5f),
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, 1.5f),
		white);
	pDebugRenderer->addLine(
		selectedLight->second->positionXYZ + glm::vec3(1.5f, 6.0f, -1.5f),
		selectedLight->second->positionXYZ + glm::vec3(-1.5f, 6.0f, -1.5f),
		white);
	if (selectedLight->second->type == 1.0f)
	{
		glm::vec3 spotNormal = selectedLight->second->getCurrentAT();
		spotNormal = glm::normalize(spotNormal);
		pDebugRenderer->addLine(
			selectedLight->second->positionXYZ,
			selectedLight->second->positionXYZ + (spotNormal*2.0f),
			yellow);
	}
}

void tools::drawGameObjectXYZ(cDebugRenderer* pDebugRenderer)
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

std::string tools::GLMvec3toString(glm::vec3 theGLMvec3)
{
	std::stringstream out;
	out << theGLMvec3.x << ", " << theGLMvec3.y << ", " << theGLMvec3.z;
	return out.str();
}

void tools::setWindowTitle(std::stringstream* ssTitle)
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
			<< " posXYZ: " << GLMvec3toString(selectedLight->second->positionXYZ)
			<< " linearA: " << selectedLight->second->LinearAtten
			<< " quadA: " << selectedLight->second->QuadraticAtten
			<< " inner: " << selectedLight->second->innerAngle
			<< " outer: " << selectedLight->second->outerAngle;
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

void tools::drawPyramidPlayer(cDebugRenderer* pDebugRenderer)
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

void tools::makeSkullEyesFlicker()
{
	//std::map<std::string, cLight> g_map_pLights;
	std::map<std::string, cLight*>::iterator itLite;

	if (timer >= 0.1)
	{
		timer = 0.0;
		itLite = ::g_map_pLights.find("candleLight1");
		if (itLite != g_map_pLights.end())
		{
			itLite->second->QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		itLite = ::g_map_pLights.find("candleLight2");
		if (itLite != g_map_pLights.end())
		{
			itLite->second->QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		itLite = ::g_map_pLights.find("candleLight3");
		if (itLite != g_map_pLights.end())
		{
			itLite->second->QuadraticAtten = randInRange(0.0001f, 0.001f);
		}
		makeCameraDroneAround(isDroneOn);
	}
}

void tools::makeCameraDroneAround(bool isDroneOn)
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

std::vector<cGameObject*> tools::getWorldMapAsVector()
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

std::vector<cGameObject*> tools::getClosestTransparentObjectsAsVector()
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
void tools::printGameObject(cGameObject* theGO)
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

void tools::makeTransparentObjectsMap()
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
void tools::CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld,cMesh& mesh_transformedInWorld)
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

void tools::drawAABBs()
{
	std::map<unsigned long long, cAABB*>::iterator itAABB;
	for (itAABB = ::g_mapAABBs_World.begin(); itAABB != ::g_mapAABBs_World.end(); itAABB++)
	{
		itAABB->second->drawAABBSelf(::pDebugRenderer);
	}
}

void tools::drawSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime)
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

void tools::duplicateSphere(glm::vec3 position, std::string texture, float sscale, float alphaT, float lifetime)
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

void tools::lifetimeValidation(cGameObject* pCurObject)
{
	if (pCurObject->tag == "lifetime")
	{
		if (pCurObject->lifetime < 1.0f)
		{
			::g_map_GameObjects.erase(pCurObject->friendlyName);
		}
		pCurObject->lifetime -= 1.0f;
	}
}

void tools::SetUpTextureBindingsForObject(cGameObject* pCurrentObject, GLint shaderProgID)
{

	//// Tie the texture to the texture unit
	//GLuint texSamp0_UL = ::pTextureManager->getTextureIDFromName("Pizza.bmp");
	//glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	//glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	// Tie the texture to the texture unit
	GLuint texSamp0_UL = ::pTextureManager->getTextureIDFromName(pCurrentObject->textures[0]);
	glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp1_UL = ::pTextureManager->getTextureIDFromName(pCurrentObject->textures[1]);
	glActiveTexture(GL_TEXTURE1);				// Texture Unit 1
	glBindTexture(GL_TEXTURE_2D, texSamp1_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp2_UL = ::pTextureManager->getTextureIDFromName(pCurrentObject->textures[2]);
	glActiveTexture(GL_TEXTURE2);				// Texture Unit 2
	glBindTexture(GL_TEXTURE_2D, texSamp2_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp3_UL = ::pTextureManager->getTextureIDFromName(pCurrentObject->textures[3]);
	glActiveTexture(GL_TEXTURE3);				// Texture Unit 3
	glBindTexture(GL_TEXTURE_2D, texSamp3_UL);	// Texture now assoc with texture unit 0

	// Tie the texture units to the samplers in the shader
	GLint textSamp00_UL = glGetUniformLocation(shaderProgID, "textSamp00");
	glUniform1i(textSamp00_UL, 0);	// Texture unit 0

	GLint textSamp01_UL = glGetUniformLocation(shaderProgID, "textSamp01");
	glUniform1i(textSamp01_UL, 1);	// Texture unit 1

	GLint textSamp02_UL = glGetUniformLocation(shaderProgID, "textSamp02");
	glUniform1i(textSamp02_UL, 2);	// Texture unit 2

	GLint textSamp03_UL = glGetUniformLocation(shaderProgID, "textSamp03");
	glUniform1i(textSamp03_UL, 3);	// Texture unit 3


	GLint tex0_ratio_UL = glGetUniformLocation(shaderProgID, "tex_0_3_ratio");
	glUniform4f(tex0_ratio_UL,
		pCurrentObject->textureRatio[0],		// 1.0
		pCurrentObject->textureRatio[1],
		pCurrentObject->textureRatio[2],
		pCurrentObject->textureRatio[3]);

	{
		//textureWhatTheWhat
		GLuint texSampWHAT_ID = ::pTextureManager->getTextureIDFromName("WhatTheWhat.bmp");
		glActiveTexture(GL_TEXTURE13);				// Texture Unit 13
		glBindTexture(GL_TEXTURE_2D, texSampWHAT_ID);	// Texture now assoc with texture unit 0

		GLint textureWhatTheWhat_UL = glGetUniformLocation(shaderProgID, "textureWhatTheWhat");
		glUniform1i(textureWhatTheWhat_UL, 13);	// Texture unit 13
	}



	return;
}