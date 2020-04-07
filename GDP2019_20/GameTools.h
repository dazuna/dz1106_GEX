#pragma once
#include <glm/vec3.hpp>

struct GameTools
{
	static float worldScale;
	static glm::vec3 coordToWorldPos(int i, int j);
	static void init();
	static void displaySelectedAlly();
	static bool validCoord(int x, int y);
	static void update(float dt);	
};
