#pragma once
#include "cGameObject.h"
#include <nlohmann/json.hpp>

struct GameUnit
{
	// where the unit currently is
	int coord_x = -1, coord_y = -1,
	// where it is moving toward during "running"
		target_x = -1, target_y = -1;
	std::string type, state = "waiting";
	// Possible status
	// "waiting" | "walking" | "attacking" | "inactive"
	bool hasAttacked = false;
	cGameObject* gameObj = nullptr;
	// TODO: change this per type of unit
	int health = 10, range = 1,
		movement = 5, rest_movement = 5;

	float timer = 0;
	int wait = 0;
	
	nlohmann::json toJSON();
	/* The player tells a unit to move in a certain direction */
	bool moveAction(int dir_x, int dir_y);
	/* Unit attacks a unit selected by the cursor, if it's in range*/
	bool attkAction();
	/*
	 * Attack action called by the AI.
	 * It checks against the allyArmy and does not zooms the camera
	 */
	bool enemyAttack();
	/* Action performed when unit gets hit */
	void getHitAction(int dir_x, int dir_y);
	void update(float dt);

	int getDistToCoord(int tar_x, int tar_y);
	void reset();
};

typedef std::vector<GameUnit*> vUnits;
