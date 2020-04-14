#pragma once
#include "GameArmies.h"


struct GameEvents
{	
	// saves complete army states
	typedef std::vector<GameUnit> vecGameUnit;
	typedef std::vector<std::vector<GameUnit>> vGameArmies;
	
	static vGameArmies allyStates;
	static vGameArmies enemyStates;
	static int stepsBack;
	/*
	 * State for managing the animation of the effect
	 * and the controls for rewinding or forwarding events
	 *
	 * Posible values: "inactive", "growing", "active", "shrinking"
	 */
	static std::string effectState;
	static float effectTimer;

	// save all units each action
	static void saveAllies();
	static void saveEnemies();
	// saves both enemiees and allies 
	static void saveGameState();
	// restores to previous state of game (n steps)

	/*
	 * Grows or shrinks the sphere for the negative effect
	 * and executes the automatic states transitions
	 */
	static void update(float dt);

	/*
	 * Process inputs related to rewinding or forwarding events
	 */
	static void inputListen(int key, int action, int mods);

	/*
	 * Deletes the states further than the one we're showing
	 */
	static void commitState();

	static void deleteHistory();

	/*
	 * Shows the state `stepsBack` steps before.
	 */
	static void goBackInTime(int stepsBack);
};
