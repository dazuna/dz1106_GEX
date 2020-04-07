#pragma once
#include <string>
#include "GamePathFinding.h"

struct EnemyAI
{
	/*
	 * Possible states:
	 * "inactive", "moving", "attacking"
	 */
	static std::string state;
	static GamePathFinding::nodeVec route;
	static GamePathFinding::cNode* target;
	static bool nextEnemy();
	static void end();

	static void update();
};
