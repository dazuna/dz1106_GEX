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

	nlohmann::json toJSON();
	/*
	 * The player tells a unit to move in a certain direction
	 */
	bool moveAction(int dir_x, int dir_y);
	void update();
};

typedef std::vector<GameUnit*> vUnits;
