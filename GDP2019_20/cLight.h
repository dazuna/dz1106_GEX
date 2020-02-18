#pragma once
#include "GLCommon.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "bRotatable/bRotatable.hpp"

class cLight : public bRotatable
{
private:
	/* Shader Stuff */
	std::string shaderVar;
	int lightID;

	GLuint shaderProgID;
	GLint ULposition;
	GLint ULdiffuse;
	GLint ULspecular;
	GLint ULatten;
	GLint ULdirection;
	GLint ULparam1;
	GLint ULparam2;
	/* /Shader Stuff */

public:
	/* Modifiable stuff */
	std::string name;
	glm::vec3 positionXYZ = glm::vec3(0.0f, 20.0f, 0.0f);
	float ConstAtten = 0.0000001f;
	float LinearAtten = 0.03f;
	float QuadraticAtten = 0.0000001f;
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f);
	// param1 x = lightType, y = inner angle, z = outer angle, w = TBD
	// 0 = pointlight // 1 = spotlight // 2 = directionallight
	float type = 0.0f;
	float innerAngle = 0.0f;
	float outerAngle = 0.0f;
	float lightW = 0.0f;
	float lightSwitch = 1.0f;
	/* /Modifiable stuff*/
	cLight();
	cLight(cLight* newLight);
	cLight(
		std::string shaderLocation,
		std::string name,
		GLuint shaderID,
		int ID);
	std::string getName();
	void getUniforms(int ID);
	void setUniforms();
};
