#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

quat RotationBetweenVectors(vec3 start, vec3 dest);
quat LookAt(vec3 direction, vec3 desiredUp);
quat RotateTowards(quat q1, quat q2, float maxAngle);