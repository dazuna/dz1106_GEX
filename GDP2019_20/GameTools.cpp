#include "GameTools.h"
#include "Terrain.h"
#include "Units.h"
#include "ImGUI_utils.h"
#include "GameCursor.h"

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
	GameCursor::init();
	GameCursor::setPosition(
		(*GameUnits::selectedAlly)->coord_x,
		(*GameUnits::selectedAlly)->coord_y
	);
}

void GameTools::displaySelectedAlly()
{
	if (GameUnits::selectedAlly != GameUnits::allyUnits.end())
	{
		auto jAlly = (*GameUnits::selectedAlly)->toJSON();
		jAlly["title"] = "Selected unit";
		ImGUI_utils::displayJSON(jAlly);
	}
}

bool GameTools::validCoord(int x, int y)
{
	return x < Terrain::width && x >= 0 &&
		y < Terrain::height && y >= 0;
}
