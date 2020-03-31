#pragma once
#include <string>
#include <vector>
#include "definitions.h"
#include <nlohmann/json.hpp>
#include "Terrain.h"

struct GameUnit
{
	int coord_x = -1, coord_y = -1;
	std::string type;
	cGameObject* gameObj = nullptr;
	// TODO: change this per type of unit
	int health = 10, range = 1, movement = 5;

	nlohmann::json toJSON()
	{
		nlohmann::json res;
		res["type"] = type;
		res["x"] = coord_x + 1;
		res["y"] = Terrain::height - coord_y;
		res["health"] = health;
		res["range"] = range;
		res["mov"] = movement;
		return res;
	}
};

typedef std::vector<GameUnit*> vUnits;

struct GameUnits
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
};

