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

cLight::cLight(cLight* newLight)
{
	this->LinearAtten = newLight->LinearAtten;
    this->QuadraticAtten = newLight->QuadraticAtten;
    this->diffuse = newLight->diffuse;
	this->setOrientation(newLight->getQOrientation());
    this->innerAngle = newLight->innerAngle;
    this->lightSwitch = newLight->lightSwitch;
    this->lightW = newLight->lightW;
    this->name = (newLight->name+"_a"+std::to_string(newLight->lightID++));
    this->outerAngle = newLight->outerAngle;
    this->positionXYZ = newLight->positionXYZ;
    this->specular = newLight->specular;
    this->type = newLight->type;
	this->shaderVar=newLight->shaderVar;
	this->shaderProgID=newLight->shaderProgID;
	this->lightID=newLight->lightID;
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
	glUniform4f(ULdirection, getCurrentAT().x, getCurrentAT().y, getCurrentAT().z, 1.0f);	// White
	glUniform4f(ULparam1, type, innerAngle, outerAngle, 1.0f);
	glUniform4f(ULparam2, lightSwitch, 0.0f, 0.0f, 1.0f);
	glUniform4f(ULatten, 0.0f,  // constant attenuation
		LinearAtten,  // Linear 
		QuadraticAtten,	// Quadratic 
		1000000.0f);	// Distance cut off
}
