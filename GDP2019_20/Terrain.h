#pragma once
#include <string>
#include <vector>
#include "definitions.h"

typedef std::vector<std::vector<std::string>> matTerrain;

struct Terrain
{
	static matTerrain terrainGrid;
	static unsigned width, height;
	static std::string getTypeFromColor(unsigned char r, unsigned char g, unsigned char b);
	static bool loadTerrain(std::string filename);
	static void setTerrainObjects();
};

