#include "GameUnit.h"
#include "Terrain.h"
#include "GameTools.h"
#include "GameArmies.h"

nlohmann::json GameUnit::toJSON()
{
	nlohmann::json res;
	res["type"] = type;
	res["x"] = coord_x + 1;
	res["y"] = Terrain::height - coord_y;
	res["health"] = health;
	res["range"] = range;
	res["rest movement"] = rest_movement;
	return res;
}

bool GameUnit::moveAction(int dir_x, int dir_y)
{
	// The unit is waiting for an action
	if (state != "waiting") return false;
	// The coord is inside the board
	int new_x = coord_x + dir_x;
	int new_y = coord_y + dir_y;
	if (!GameTools::validCoord(new_x, new_y)) return false;
	auto coordType = Terrain::terrainGrid[new_x][new_y];
	// the terrain is walkable
	if (coordType == "wall" || coordType == "water") return false;
	// is the space free
	if (GameArmies::isCoordOccupied(new_x, new_y)) return false;
	// The unit has enough movement left to enter that terrain
	if (coordType == "tree" && rest_movement < 2) return false;
	if (coordType == "ground" && rest_movement < 1) return false;

	// We're good to move
	target_x = new_x; target_y = new_y;
	if (coordType == "tree") rest_movement -= 2;
	if (coordType == "ground") rest_movement -= 1;
	state = "walking";
	gameObj->pAS->setActiveAnimation("Run");
	auto targetPos = GameTools::coordToWorldPos(new_x, new_y);
	gameObj->setAT(targetPos - gameObj->positionXYZ);
	gameObj->velocity = gameObj->getCurrentAT() * GameTools::worldScale * 3.f;
	return true;
}

void GameUnit::update()
{
	if (state == "walking")
	{
		auto targetPos = GameTools::coordToWorldPos(target_x, target_y);
		if (glm::distance(gameObj->positionXYZ, targetPos) < 1.5f)
		{
			// put the unit exactly at the end
			gameObj->positionXYZ = targetPos;
			coord_x = target_x; coord_y = target_y;
			target_x = -1; target_y = -1;
			// stop it
			gameObj->velocity = glm::vec3(0);
			// make it look stopped
			gameObj->pAS->setActiveAnimation("Idle");
			state = "waiting";
		}
	}
}
