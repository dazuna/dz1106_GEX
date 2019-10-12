#include "cLight.h"

cLight::cLight()
{
}

cLight::cLight(
	std::string shaderLocation,
	std::string meshName,
	GLuint shaderID,
	int ID)
{
	shaderVar = shaderLocation;
	name = meshName;
	shaderProgID = shaderID;
	lightID = ID;
	getUniforms(ID);
}

std::string cLight::getName()
{
	return name;
}

void cLight::getUniforms(int ID)
{
	ULposition = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].position")).c_str());
	ULdiffuse = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].diffuse")).c_str());
	ULspecular = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].specular")).c_str());
	ULatten = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].atten")).c_str());
	ULdirection = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].direction")).c_str());
	ULparam1 = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].param1")).c_str());
	ULparam2 = glGetUniformLocation(shaderProgID, (shaderVar+("["+std::to_string(ID)+"].param2")).c_str());
}

void cLight::setUniforms()
{
	glUniform4f(ULposition,positionXYZ.x,positionXYZ.y,positionXYZ.z,1.0f);
	glUniform4f(ULdiffuse,diffuse.r,diffuse.g,diffuse.b, 1.0f);	// White
	glUniform4f(ULspecular,specular.r,specular.g,specular.b, 1.0f);	// White
	glUniform4f(ULdirection,direction.r,direction.g,direction.b, 1.0f);	// White
	glUniform4f(ULparam1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
	glUniform4f(ULparam2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f);
	glUniform4f(ULatten, 0.0f,  // constant attenuation
		LinearAtten,  // Linear 
		QuadraticAtten,	// Quadratic 
		1000000.0f);	// Distance cut off
}
