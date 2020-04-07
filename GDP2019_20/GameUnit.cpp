#include "GameUnit.h"

#include "cFlyCamera/cFlyCamera.h"
#include "Terrain.h"
#include "GameTools.h"
#include "GameArmies.h"
#include "GameCursor.h"
#include "GameEvents.h"

nlohmann::json GameUnit::toJSON()
{
	nlohmann::json res;
	auto cam = cFlyCamera::getTheCamera();
	res["type"] = type;
	res["x"] = coord_x + 1;
	res["y"] = Terrain::height - coord_y;
	res["health"] = health;
	res["range"] = range;
	res["rest movement"] = rest_movement;
	res["state"] = state;
	res["camState"] = cam->state;
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

	// save state
	GameEvents::saveGameState();
	
	// We're good to move
	target_x = new_x; target_y = new_y;
	if (coordType == "tree") rest_movement -= 2;
	if (coordType == "ground") rest_movement -= 1;
	state = "walking";
	gameObj->pAS->setActiveAnimation("Run");
	auto targetPos = GameTools::coordToWorldPos(new_x, new_y);
	gameObj->setAT(targetPos - gameObj->positionXYZ);
	gameObj->velocity = gameObj->getCurrentAT() * GameTools::worldScale * 3.f;
	GameCursor::setCoordinates(new_x,new_y);
	
	return true;
}

bool GameUnit::attkAction()
{
	// The unit is waiting for an action
	if (state != "waiting") return false;
	// The coord is inside the board
	int new_x = GameCursor::coord_x;
	int new_y = GameCursor::coord_y;
	if (!GameTools::validCoord(new_x, new_y)) return false;

	auto dist = getDistToCoord(new_x,new_y);
	std::cout << "dist to cursor: " << dist << std::endl;
	std::cout << "range: " << this->range << std::endl;
	//return false;
	if(dist > range) return false;
	auto enemy = GameArmies::getUnitByCoord(GameArmies::enemyUnits,new_x,new_y);
	if(!enemy) return false;
	
	gameObj->setAT(enemy->gameObj->positionXYZ - gameObj->positionXYZ);
	state = "wait4attacking";
	enemy->state = "wait4getHit";

	auto cam = cFlyCamera::getTheCamera();
	cam->battleTarget = gameObj->positionXYZ;
	cam->state = "zoom_in";
	
	
	return true;
}

void GameUnit::getHitAction(int dir_x, int dir_y)
{
	auto targetPos = GameTools::coordToWorldPos(dir_x, dir_y);
	gameObj->setAT(targetPos - gameObj->positionXYZ);
	state = "wait4getHit";
}

void GameUnit::update(float dt)
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
	if(state == "wait4attacking")
	{
		timer+=dt;
		if(timer > 1.f)
		{
			wait++;
			timer = 0.f;
		}
		if(wait >= 4)
		{
			gameObj->pAS->setActiveAnimation("Attack");
			state = "attacking";			
		}
	}
	if(state == "gotHit")
	{
		if(!gameObj->pAS->activeAnimation)
		{
			state = "inactive";
			wait = 0;
		}
	}
	if(state == "attacking")
	{
		if(!gameObj->pAS->activeAnimation)
		{
			/* setup camera wait */
			auto cam = cFlyCamera::getTheCamera();
			cam->state = "wait";
			cam->timeToWait = 3.f;
			cam->nextAction = "zoom_out";
			/* /setup camera wait */
			state = "inactive";
			wait = 0;
		}
	}
	if(state == "wait4getHit")
	{
		timer+=dt;
		if(timer > 1.f)
		{
			wait++;
			timer = 0.f;
		}
		if(wait >= 4)
		{
			gameObj->pAS->setActiveAnimation("GetHit");
			state = "gotHit";			
		}
	}
}

int GameUnit::getDistToCoord(int tar_x, int tar_y)
{
	int dist_x = std::abs(coord_x-tar_x);
	int dist_y = std::abs(coord_y-tar_y);
	return(dist_x+dist_y);
}
