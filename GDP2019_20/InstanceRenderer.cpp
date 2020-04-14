#include "InstanceRenderer.h"
#include "cShaderManager.h"
#include "cVAOManager.h"
#include <glad/glad.h>
#include "util/tools.h"

extern cVAOManager* pTheVAOManager;
extern GLuint shaderProgID;
std::map<std::string,InstanceRenderer*> InstanceRenderer::mapIR;

void InstanceRenderer::setVAOVariables()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vecWMs.size() * sizeof(glm::mat4), vecWMs.data(), GL_STATIC_DRAW);

	sModelDrawInfo drawInfo;	
	if (!pTheVAOManager->FindDrawInfoByModelName(gameObj->meshName, drawInfo)) 
    {return;}
    glBindVertexArray(drawInfo.VAO_ID);
    // vertex attributes
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(8); 
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(9); 
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(10); 
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(11); 
    glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(8, 1);
    glVertexAttribDivisor(9, 1);
    glVertexAttribDivisor(10, 1);
    glVertexAttribDivisor(11, 1);

    glBindVertexArray(0);
	status = "ready2Draw";
}

void InstanceRenderer::drawIR()
{
	if(status != "ready2Draw") return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// texture bindings
	tools::SetUpTextureBindingsForObject(gameObj, shaderProgID);
	auto m = glm::mat4(1);
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");
	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));
	
	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");
	glUniform3f(newColour_location,
		gameObj->objectColourRGBA.r,
		gameObj->objectColourRGBA.g,
		gameObj->objectColourRGBA.b);

	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		gameObj->objectColourRGBA.r,
		gameObj->objectColourRGBA.g,
		gameObj->objectColourRGBA.b,
		gameObj->alphaTransparency);

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

	if (gameObj->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			gameObj->objectColourRGBA.r,
			gameObj->objectColourRGBA.g,
			gameObj->objectColourRGBA.b,
			gameObj->objectColourRGBA.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
		glCullFace(GL_BACK);
	}
	//glPointSize(15.0f);

	if (gameObj->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
		glDepthMask(GL_FALSE);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
		glDepthMask(GL_TRUE);
	}

	if (gameObj->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}

	auto jGraphicEffects = gameObj->jGraphicEffects;
	if (jGraphicEffects.contains("toonOutline") &&
		jGraphicEffects["toonOutline"])
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "objectID"), gameObj->getUniqueID());
	}
	else
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "objectID"), 0);
	}
	
	if (jGraphicEffects.contains("outlineColorID"))
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "outlineColorID"), jGraphicEffects["outlineColorID"]);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "outlineColorID"), 0);
	}

	if (jGraphicEffects.contains("outlineWidth"))
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "outlineWidth"), jGraphicEffects["outlineWidth"]);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shaderProgID, "outlineWidth"), 0);
	}
	
	glUniform1f(glGetUniformLocation(shaderProgID, "isBloom"),		float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "isNightVision"),float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "shouldReflect"),float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "shoulfRefract"),float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "discardBlack"),	float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "isSkinnedMesh"),float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "debugger"),		float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "bIsSkyBox"),	float(GL_FALSE));
	glUniform1f(glGetUniformLocation(shaderProgID, "toonLighting"), float(GL_TRUE));
	glUniform1i(glGetUniformLocation(shaderProgID, "passNumber"), 0);
	// is Instanced always true here
	glUniform1f(glGetUniformLocation(shaderProgID, "isInstanced"),float(GL_TRUE));
	
	sModelDrawInfo drawInfo;	
	if (!pTheVAOManager->FindDrawInfoByModelName(gameObj->meshName, drawInfo)) {return;}
    glBindVertexArray(drawInfo.VAO_ID);
    glDrawElementsInstanced(GL_TRIANGLES, drawInfo.numberOfIndices, GL_UNSIGNED_INT, 0, vecWMs.size());
}
