#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "GameUnit.h"

struct GameArmies
{
	static vUnits allyUnits, enemyUnits;
	static vUnits::iterator selectedAlly, selectedEnemy;
	static GameUnit* getUnitByCoord(const vUnits& units, int x, int y);
	static std::string getTypeFromColor(unsigned char r, unsigned char g, unsigned char b);
	static bool loadUnits(vUnits& vUnits, std::string filename);
	static bool loadAllies(std::string filename);
	static bool loadEnemies(std::string filename);
	static void setUnitObjects();
	static void nextAlly();
	static void previousAlly();
	/*
	 * Resets the properties of the object and other stuff
	 * to show the unit as not selected
	 */
	static void deselectUnit(GameUnit* unit);
	/*
	 * Sets all of the extra things to show that a unit is selected
	 * Like moving the cursor, maybe changing the unit's color, etc
	 */
	static void selectUnit(vUnits::iterator selectedUnit);
	static bool isCoordOccupied(int x, int y);
	static void killUnits();
};

