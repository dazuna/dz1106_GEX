#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>	// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

enum class steeringType
{
	none,
    seek,
    flee,
    arrive,
    wander,
    pursue,
    evade
};

class cSteeringBehaviour
{
private:
public:
    cSteeringBehaviour(/* args */);
    ~cSteeringBehaviour();

	static glm::vec3 Update(glm::vec3 vehiclePosition,
		glm::vec3 vehicleVelocity,
		glm::vec3 target,
		glm::vec3 targetVelocity,
		float deltaTime,
		steeringType sType);

	static glm::vec3 seekBhvr(glm::vec3 vehiclePosition,
	    glm::vec3 vehicleVelocity, glm::vec3 target,
	    float deltaTime, float maxVelocity = 10.0f);
    static glm::vec3 fleeBhvr(glm::vec3 vehiclePosition,
	    glm::vec3 vehicleVelocity, glm::vec3 target,
	    float deltaTime, float maxVelocity = 10.0f);
    static glm::vec3 arriveBhvr(glm::vec3 vehiclePosition,
	    glm::vec3 vehicleVelocity, glm::vec3 target,
	    float deltaTime, float maxVelocity = 10.0f);
    static glm::vec3 wanderBhvr(glm::vec3 vehiclePosition,
	    glm::vec3 vehicleVelocity,
	    float deltaTime, float maxVelocity = 10.0f);
    static glm::vec3 pursueBhvr(
	    glm::vec3 vehiclePosition, glm::vec3 vehicleVelocity, 
	    glm::vec3 target, glm::vec3 targetVelocity,
	    float deltaTime, float maxVelocity = 10.0f);
    static glm::vec3 evadeBhvr(
	    glm::vec3 vehiclePosition, glm::vec3 vehicleVelocity,
	    glm::vec3 target, glm::vec3 targetVelocity,
	    float deltaTime, float maxVelocity = 20.0f);
};

