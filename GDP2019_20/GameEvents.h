#pragma once
#include "GameArmies.h"


struct GameEvents
{	
	// saves complete army states
	typedef std::vector<GameUnit> vecGameUnit;
	typedef std::vector<std::vector<GameUnit>> vGameArmies;
	
	static vGameArmies allyStates;
	static vGameArmies enemyStates;

	// save all units each action
	static void saveAllies();
	static void saveEnemies();
	// saves both enemiees and allies 
	static void saveGameState();
	// restores to previous state of game (n steps)
	static void goBackInTime(int stepsBack);
};
