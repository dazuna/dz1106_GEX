#include "EnemyAI.h"
#include "GameArmies.h"
#include "GameTools.h"
#include "GameCursor.h"
#include "cFlyCamera/cFlyCamera.h"

std::string EnemyAI::state = "inactive";
GamePathFinding::nodeVec EnemyAI::route;
GamePathFinding::cNode* EnemyAI::target = nullptr;

bool EnemyAI::nextEnemy()
{
	GameArmies::selectedEnemy++;
	return GameArmies::selectedEnemy != GameArmies::enemyUnits.end();
}

void EnemyAI::end()
{
	state = "inactive";
	GameArmies::selectedEnemy = GameArmies::enemyUnits.begin();
	GameTools::changeTurn();
}

void EnemyAI::update()
{
	if (state == "inactive") return;
	
	if(GameArmies::selectedEnemy == GameArmies::enemyUnits.end())
	{
		end();
	}

	auto unit = *GameArmies::selectedEnemy;

	if (state == "begin")
	{
		if (unit->state == "inactive")
		{
			if (!nextEnemy())
			{
				end();
				return;
			}
		}
		
		GameArmies::selectedEnemy = GameArmies::enemyUnits.begin();
		state = "path_finding";
	}
	
	if (state == "path_finding")
	{
		GamePathFinding pathFinder;
		target = pathFinder.Dijkstra(unit->coord_x, unit->coord_y);
		if (!target)
		{
			if (!nextEnemy())
			{
				end();
			}
			return;
		}
		route = pathFinder.getRouteTo(target);
		state = "moving";
	}

	if (state == "moving")
	{
		if (unit->state != "waiting") return;

		if (route.size() <= unit->range)
		{
			state = "attacking";
			GameCursor::setCoordinates(target->coord_x, target->coord_y);
			if (!unit->attkAction())
			{
				if (!nextEnemy())
				{
					end();
				}
				return;
			}
			return;
		}

		if (unit->rest_movement == 0)
		{
			if (!nextEnemy())
			{
				end();
			}
			else
			{
				state = "path_finding";
			}
			return;
		}

		int dir_x = route[1]->coord_x - route[0]->coord_x,
		 dir_y = route[1]->coord_y - route[0]->coord_y;

		unit->moveAction(dir_x, dir_y);
		route.erase(route.begin());
		return;
	}

	if (state == "attacking")
	{
		auto theCamera = cFlyCamera::getTheCamera();
		auto targetUnit = GameArmies::getUnitByCoord(GameArmies::allyUnits, target->coord_x, target->coord_y);
		// Bot Units has finished the attack animation
		if (unit->state == "inactive" &&
			(!targetUnit ||
			  targetUnit->state == "inactive"))
		{
			if (!nextEnemy())
			{
				end();
			}
			else
			{
				state = "path_finding";
			}
			return;
		}
		return;
	}
}
