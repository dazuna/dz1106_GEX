#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GFLW_callbacks.h"

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}



