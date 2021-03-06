#include "GameTools.h"
#include "Terrain.h"
#include "GameArmies.h"
#include "ImGUI_utils.h"
#include "GameCursor.h"
#include "GamePathFinding.h"
#include "EnemyAI.h"
#include "GameEvents.h"
#include "JsonState.h"

float GameTools::worldScale = 10.0f;
bool GameTools::isPlayerTurn = true;
bool GameTools::isFogActive = true;
std::string GameTools::winner = "";

glm::vec3 GameTools::coordToWorldPos(int i, int j)
{
	return glm::vec3(i * worldScale, 0, j * worldScale);
}

void GameTools::init()
{
	std::cout << "Initializing game stuff..." << std::endl;
	auto config = JsonState::getTheJsonState()->JSONGlobalConfig;
	if (!config.contains("mapsBaseLocation"))
	{
		std::cout << "No maps base location in global config!!" << std::endl;
		return;
	}
	auto mapsBaseLocation = config["mapsBaseLocation"].get<std::string>();
	if (!Terrain::loadTerrain(mapsBaseLocation + "terrain.png")) return;
	Terrain::setTerrainObjects();
	if (!GameArmies::loadAllies(mapsBaseLocation + "playerUnits.png")) return;
	if (!GameArmies::loadEnemies(mapsBaseLocation + "enemyUnits.png")) return;
	GameArmies::setUnitObjects();
	GameCursor::init();
	GameArmies::selectUnit(GameArmies::selectedAlly);
}

void GameTools::displaySelectedAlly()
{
	GameUnit* selectedUnit = GameArmies::getUnitByCoord(GameArmies::allyUnits, GameCursor::coord_x, GameCursor::coord_y);
	if (selectedUnit)
	{
		auto jAlly = selectedUnit->toJSON();
		jAlly["title"] = "Selected ally unit";
		ImGUI_utils::displayJSON(jAlly);
		return;
	}

	selectedUnit = GameArmies::getUnitByCoord(GameArmies::enemyUnits, GameCursor::coord_x, GameCursor::coord_y);
	if (selectedUnit)
	{
		auto jAlly = selectedUnit->toJSON();
		jAlly["title"] = "Selected enemy unit";
		ImGUI_utils::displayJSON(jAlly);
		return;
	}
}

bool GameTools::validCoord(int x, int y)
{
	return x < Terrain::width && x >= 0 &&
		y < Terrain::height && y >= 0;
}

void GameTools::update(float dt)
{
	if (winner != "")
	{
		for (auto unit : GameArmies::allyUnits)
		{
			unit->gameObj->pAS->setActiveAnimation("Dance");
		}
		for (auto unit : GameArmies::enemyUnits)
		{
			unit->gameObj->pAS->setActiveAnimation("Dance");
		}
		return;
	}
	
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
	GameEvents::update(dt);
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
	// if we want to limit the player back in time
	// power to their own turns
	//GameEvents::deleteHistory();
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

