#include "GameTools.h"
#include "Terrain.h"
#include "Units.h"

float GameTools::worldScale = 10.0f;

glm::vec3 GameTools::coordToWorldPos(int i, int j)
{
	return glm::vec3(i * worldScale, 0, j * worldScale);
}

void GameTools::init()
{
	std::cout << "Initializing game stuff..." << std::endl;
	Terrain::loadTerrain("./assets/textures/terrain.png");
	Terrain::setTerrainObjects();
	GameUnits::loadAllies("./assets/textures/playerUnits.png");
	GameUnits::loadEnemies("./assets/textures/enemyUnits.png");
	GameUnits::setUnitObjects();
}
