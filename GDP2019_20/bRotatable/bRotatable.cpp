#include "bRotatable.hpp"

// always the same since it's the original loading state
glm::vec3 bRotatable::ogAT = glm::vec3(0,0,1);
glm::vec3 bRotatable::ogUP = glm::vec3(0,1,0);

// Magic Methods
glm::vec3 bRotatable::getCurrentAT()
{
	return (this->qOrientation * this->ogAT);
}
glm::vec3 bRotatable::getCurrentUP()
{
	return (this->qOrientation * this->ogUP);
}
// Overwrite the orientation
void bRotatable::setOrientation(glm::vec3 EulerAngleDegreesXYZ)
{
	glm::vec3 EulerAngleRadians;
	EulerAngleRadians = glm::radians(EulerAngleDegreesXYZ);
	this->qOrientation = glm::quat(EulerAngleRadians);
}
void bRotatable::setOrientation(glm::quat qAngle)
{
	this->qOrientation = qAngle;
}
// Updates the existing angle
void bRotatable::updateOrientation(glm::vec3 EulerAngleDegreesXYZ)
{
	glm::vec3 EulerAngleRadians;
	EulerAngleRadians = glm::radians(EulerAngleDegreesXYZ);
	glm::quat angleChange = glm::quat(EulerAngleRadians);
	this->qOrientation *= angleChange;
}
void bRotatable::updateOrientation(glm::quat qAngle)
{
	this->qOrientation *= qAngle;
}

// For saving it in the JSON
glm::vec3 bRotatable::getEulerAngle(void)
{
	glm::vec3 EulerAngle = glm::eulerAngles(this->qOrientation);
	EulerAngle = glm::degrees(EulerAngle);
	return EulerAngle;
}

void bRotatable::setAT(glm::vec3 direction)
{
	this->qOrientation = RotationBetweenVectors(this->ogAT, direction);
}

glm::vec3 bRotatable::getOG_AT()
{
	return this->ogAT;
}

glm::vec3 bRotatable::getRight()
{
	glm::vec3 right(0.f);
	right = glm::cross(getCurrentAT(),getCurrentUP());
	return glm::normalize(right);
}

