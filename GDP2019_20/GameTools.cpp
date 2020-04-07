#include "GameTools.h"
#include "Terrain.h"
#include "GameArmies.h"
#include "ImGUI_utils.h"
#include "GameCursor.h"
#include "GamePathFinding.h"
#include "EnemyAI.h"

float GameTools::worldScale = 10.0f;
bool GameTools::isPlayerTurn = true;

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
	GameArmies::selectUnit(GameArmies::selectedAlly);
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
	for (auto unit : GameArmies::allyUnits)
	{
		unit->update(dt);
	}
	for (auto unit : GameArmies::enemyUnits)
	{
		unit->update(dt);
	}
	if (!isPlayerTurn)
	{
		EnemyAI::update();
	}
}

bool GameTools::isCoordWalkable(int x, int y)
{
	// The coord is inside the board
	if (!validCoord(x, y)) return false;
	auto terrainType = Terrain::terrainGrid[x][y];
	// the terrain is walkable
	if (!Terrain::isTerrainWalkable(terrainType)) return false;
	// is the space free
	if (GameArmies::isCoordOccupied(x, y)) return false;
	return true;
}

void GameTools::changeTurn()
{
	isPlayerTurn = !isPlayerTurn;
	const auto& armyInTurn = isPlayerTurn ? GameArmies::allyUnits : GameArmies::enemyUnits;
	for (auto unit : armyInTurn)
	{
		unit->reset();
	}
	if (!isPlayerTurn)
	{
		EnemyAI::state = "begin";
	}
	else
	{
		GameArmies::selectUnit(GameArmies::selectedAlly);
	}
}
