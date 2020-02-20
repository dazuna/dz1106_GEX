#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <iostream>
#include "../quaternion_utils/quaternion_utils.hpp"

class bRotatable
{
public:
    glm::quat getQOrientation() {return this->qOrientation;}
    glm::vec3 getCurrentAT();
    glm::vec3 getCurrentUP();
    // Overwrite the orientation
	void setOrientation(glm::vec3 EulerAngleDegreesXYZ);
	void setOrientation(glm::quat qAngle);
	// Updates the existing angle
	void updateOrientation(glm::vec3 EulerAngleDegreesXYZ);
	void updateOrientation(glm::quat qAngle);
	glm::vec3 getEulerAngle(void);
	// setAT ... for light manipulation
	void setAT(glm::vec3 direction);
	glm::vec3 getOG_AT();
	glm::vec3 getRight();
private:
    glm::quat qOrientation;
    static glm::vec3 ogAT;
    static glm::vec3 ogUP;
};
