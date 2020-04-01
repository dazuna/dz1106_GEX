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
	static bool isCoordOccupied(int x, int y);
};

