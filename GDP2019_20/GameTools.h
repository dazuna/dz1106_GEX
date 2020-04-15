#pragma once
#include <glm/vec3.hpp>
#include <string>

struct GameTools
{
	static float worldScale;
	static bool isPlayerTurn;
	static std::string winner;
	static glm::vec3 coordToWorldPos(int i, int j);
	static void init();
	static void displaySelectedAlly();
	static bool validCoord(int x, int y);
	static void update(float dt);
	/*
	 * Considers the following:
	 * - If the coordinate is inside the bounds of the terrain
	 * - The type of terrain
	 * - If there's a unit in there
	 * TODO: If we ever make flying units, this needs to consider the type of unit asking
	 */
	static bool isCoordWalkable(int x, int y);
	static void changeTurn();
};
