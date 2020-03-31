#pragma once
#include <glm/vec3.hpp>

struct GameTools
{
	static float worldScale;
	static glm::vec3 coordToWorldPos(int i, int j);
	static void init();
};
