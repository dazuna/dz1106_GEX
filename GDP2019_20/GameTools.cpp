#include "GameTools.h"
#include "Terrain.h"
#include "GameArmies.h"
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
	Terrain::loadTerrain("./assets/textures/maps/small/terrain.png");
	Terrain::setTerrainObjects();
	GameArmies::loadAllies("./assets/textures/maps/small/playerUnits.png");
	GameArmies::loadEnemies("./assets/textures/maps/small/enemyUnits.png");
	GameArmies::setUnitObjects();
	GameCursor::init();
	GameCursor::setCoordinates(
		(*GameArmies::selectedAlly)->coord_x,
		(*GameArmies::selectedAlly)->coord_y
	);
}

void GameTools::displaySelectedAlly()
{
	if (GameArmies::selectedAlly != GameArmies::allyUnits.end())
	{
		auto jAlly = (*GameArmies::selectedAlly)->toJSON();
		jAlly["title"] = "Selected unit";
		ImGUI_utils::displayJSON(jAlly);
	}
}

bool GameTools::validCoord(int x, int y)
{
	return x < Terrain::width && x >= 0 &&
		y < Terrain::height && y >= 0;
}

void GameTools::update(float dt)
{
	// TODO: Maybe just update the units of the active turn??
	for (auto unit : GameArmies::allyUnits)
	{
		unit->update(dt);
	}
	for (auto unit : GameArmies::enemyUnits)
	{
		unit->update(dt);
	}
}
