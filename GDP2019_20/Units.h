#pragma once
#include <string>
#include <vector>
#include "definitions.h"

struct GameUnit
{
	int coord_x = -1, coord_y = -1;
	std::string type;
	cGameObject* gameObj = nullptr;
	int health, range, movement;
};

typedef std::vector<GameUnit*> vUnits;

struct GameUnits
{
	static vUnits playerUnits, enemyUnits;
	static GameUnit* getUnitByCoord(const vUnits& units, int x, int y);
	static std::string getTypeFromColor(unsigned char r, unsigned char g, unsigned char b);
	static bool loadUnits(vUnits& vUnits, std::string filename);
	static bool loadAllies(std::string filename);
	static bool loadEnemies(std::string filename);
	static void setUnitObjects();
};

