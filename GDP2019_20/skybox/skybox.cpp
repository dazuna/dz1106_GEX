#include "skybox.h"

#include "../globalStuff.h"
#include "../cGameObject.h"
#include "../cVAOManager.h"
#include "../TextureManager/cBasicTextureManager.h"
#include "../util/tools.h"
#include <glm/glm.hpp>
#include <string>

bool createSkyBoxObject()
{
	cMesh* tempMesh = new cMesh();
	pTheModelLoader->LoadPlyModel("assets/models/Sphere_Radius_1_XYZ_n_uv.ply", *tempMesh);
	sModelDrawInfo drawInfo;
	pTheVAOManager->LoadModelIntoVAO("skybox", *tempMesh, drawInfo, shaderProgID);
	::pSkyBox->meshName = "skybox";
	::pSkyBox->scale = 1.0f;
	return true;
}

void drawSkyBox(glm::vec3 positionSkyBox)
{
	// texture bindings
	// SetUpTextureBindingsForObject(::pSkyBox, shaderProgID);
	glUseProgram(shaderProgID);
	//::pSkyBox->positionXYZ = ::g_pFlyCamera->eye;
	::pSkyBox->positionXYZ = positionSkyBox;
	//::pSkyBox->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 m = tools::calculateWorldMatrix(::pSkyBox);
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));
	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

	//	uniform vec4 debugColour;
	//	uniform bool bDoNotLight;
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH);

	GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgID, "bIsSkyBox");
	glCullFace(GL_FRONT_AND_BACK);
	glUniform1f(bIsSkyBox_UL, (float)GL_TRUE);
	GLuint skyBoxTextureID = ::pTextureManager->getTextureIDFromName("space");
	glActiveTexture(GL_TEXTURE26);				// Texture Unit 26
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0
	// Tie the texture units to the samplers in the shader
	GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgID, "skyBox");
	glUniform1i(skyBoxSampler_UL, 26);	// Texture unit 26

	sModelDrawInfo drawInfo;
	if (pTheVAOManager->FindDrawInfoByModelName(::pSkyBox->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	glUniform1f(bIsSkyBox_UL, (float)GL_FALSE);
}

void setSkyBoxTexture()
{
	::pTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;

	if (::pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"SpaceBox_right1_posX.png", "SpaceBox_left2_negX.png",
		"SpaceBox_top3_posY.png", "SpaceBox_bottom4_negY.png",
		"SpaceBox_front5_posZ.png", "SpaceBox_back6_negZ.png", true, errorString))
	{
		std::cout << "Space skybox loaded" << std::endl;
	}
	else
	{
		std::cout << "OH NO! " << errorString << std::endl;
	}
	
	//if (::pTextureManager->CreateCubeTextureFromBMPFiles("space",
	//	"SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp",
	//	"SpaceBox_top3_posY.bmp", "SpaceBox_bottom4_negY.bmp",
	//	"SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, errorString))
	//{
	//	std::cout << "Space skybox loaded" << std::endl;
	//}
	//else
	//{
	//	std::cout << "OH NO! " << errorString << std::endl;
	//}
	
	//if (::pTextureManager->CreateCubeTextureFromBMPFiles("space",
	//	"city_rt.bmp", "city_lf.bmp",
	//	"city_up.bmp", "city_dn.bmp",
	//	"city_ft.bmp", "city_bk.bmp", true, errorString))
	//{
	//	std::cout << "Space skybox loaded" << std::endl;
	//}
	//else
	//{
	//	std::cout << "OH NO! " << errorString << std::endl;
	//}
}
