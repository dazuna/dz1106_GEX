#include "GameArmies.h"
#include "LodePNG/lodepng.h"
#include <iostream>
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"
#include "GameTools.h"
#include "GameCursor.h"

vUnits GameArmies::allyUnits, GameArmies::enemyUnits;
vUnits::iterator GameArmies::selectedAlly = GameArmies::allyUnits.begin(),
GameArmies::selectedEnemy = GameArmies::enemyUnits.begin();

GameUnit* GameArmies::getUnitByCoord(const vUnits& units, int x, int y)
{
	for (auto unit : units)
	{
		if (unit->coord_x == x && unit->coord_y == y &&
			unit->health > 0)
		{
			return unit;
		}
	}
	return nullptr;
}

std::string GameArmies::getTypeFromColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0) return "sword";
	if (r == 0 && g == 255 && b == 0) return "archer";
	if (r == 0 && g == 0 && b == 255) return "wizard";
	return "";
}

bool GameArmies::loadUnits(vUnits& vUnits, std::string filename)
{
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error)
	{
		std::cout << "error loading the terrain map: " << error << ": " << lodepng_error_text(error) << filename << std::endl;
		return false;
	}

	auto idx = 0;
	for (auto y = 0; y < height; y++)
	{
		for (auto x = 0; x < width; x++)
		{
			auto red = image[idx++];
			auto green = image[idx++];
			auto blue = image[idx++];
			auto alpha = image[idx++];
			auto type = getTypeFromColor(red, green, blue);
			if (type != "")
			{
				auto newUnit = new GameUnit();
				newUnit->coord_y = height - y - 1;
				newUnit->coord_x = x;
				newUnit->type = type;
				vUnits.push_back(newUnit);
			}
		}
	}
	int i = 0;
	i++;
	return true;
}

bool GameArmies::loadAllies(std::string filename)
{
	auto res = loadUnits(allyUnits, filename);
	if (res)
	{
		selectedAlly = allyUnits.begin();
	}
	for (auto ally : allyUnits)
	{
		ally->jAdditionalInfo["team"] = "player";
	}
	return res;
}

bool GameArmies::loadEnemies(std::string filename)
{
	auto res = loadUnits(enemyUnits, filename);
	if (res)
	{
		selectedEnemy = enemyUnits.begin();
	}
	for (auto enemy : enemyUnits)
	{
		enemy->jAdditionalInfo["team"] = "enemy";
	}
	return res;
}

cGameObject* createUnitObj(GameUnit* unit)
{
	cGameObject* newUnit;
	if (unit->type == "sword")
	{
		if (!tools::pFindObjectByFriendlyNameMap("knight"))
		{
			std::cout << "No knight model!!" << std::endl;
			return nullptr;
		}
		newUnit = new cGameObject(::g_map_GameObjects["knight"]);
	} else return nullptr;
	newUnit->isVisible = true;
	// The units are a bit larger than we want 
	newUnit->scale = GameTools::worldScale / 250;
	newUnit->positionXYZ = GameTools::coordToWorldPos(unit->coord_x, unit->coord_y);
	auto theSceneManager = cSceneManager::getTheSceneManager();
	theSceneManager->scenesVector[0]->addGameObject(newUnit);
	::g_map_GameObjects[newUnit->friendlyName] = newUnit;
	unit->gameObj = newUnit;

	// Outline color

	// The color of the outline is specified by index of an array defined in
	// the fragment shader. 1 is blue, 2 is red.
	int colorID = (unit->jAdditionalInfo["team"] == "player") ?
		1 : 2;
	unit->gameObj->jGraphicEffects["outlineColorID"] = colorID;
	
	return newUnit;
}

void GameArmies::setUnitObjects()
{
	for (auto unit : allyUnits)
	{
		cGameObject* newUnit = createUnitObj(unit);
		if (newUnit)
		{
			newUnit->setAT(glm::vec3(1, 0, 0));
			//newUnit->textures[0] = "Blue.png";
		}
	}
	
	for (auto unit : enemyUnits)
	{
		cGameObject* newUnit = createUnitObj(unit);
		if (newUnit)
		{
			newUnit->setAT(glm::vec3(-1, 0, 0));
			//newUnit->textures[0] = "Red.png";
		}
	}
}

void GameArmies::deselectUnit(GameUnit* unit)
{
	unit->gameObj->jGraphicEffects["outlineWidth"] = 2;
	unit->gameObj->jGraphicEffects["outlineColorID"] = 1;
}

void GameArmies::nextAlly()
{
	deselectUnit(*selectedAlly);
	selectedAlly++;
	if (selectedAlly == allyUnits.end())
	{
		selectedAlly = allyUnits.begin();
	}
	selectUnit(selectedAlly);
}

void GameArmies::previousAlly()
{
	deselectUnit(*selectedAlly);
	if (selectedAlly == allyUnits.begin())
	{
		selectedAlly = allyUnits.end();
	}
	selectedAlly--;
	selectUnit(selectedAlly);
}

void GameArmies::selectUnit(vUnits::iterator selectedUnit)
{
	GameCursor::setCoordinates((*selectedUnit)->coord_x, (*selectedUnit)->coord_y);
	(*selectedUnit)->gameObj->jGraphicEffects["outlineWidth"] = 3;
	(*selectedUnit)->gameObj->jGraphicEffects["outlineColorID"] = 3;
}

bool GameArmies::isCoordOccupied(int x, int y)
{
	for (auto unit : allyUnits)
	{
		if (unit->coord_y == y && unit->coord_x == x)
		{
			return true;
		}
	}
	for (auto unit : enemyUnits)
	{
		if (unit->coord_y == y && unit->coord_x == x)
		{
			return true;
		}
	}
	return false;
}

void _killUnits(vUnits& army, vUnits::iterator& selectedUnit)
{
	vUnits::iterator itUnit = army.begin();
	for (;itUnit != army.end();)
	{
		if ((*itUnit)->health <= 0)
		{
			(*itUnit)->gameObj->isVisible = false;
			auto tempItUnit = army.erase(itUnit);
			if (itUnit == selectedUnit)
			{
				selectedUnit = tempItUnit;
			}
			itUnit = tempItUnit;			
		}
		else
		{
			itUnit++;
		}
	}
}

void GameArmies::killUnits()
{
	_killUnits(allyUnits, selectedAlly);
	_killUnits(enemyUnits, selectedEnemy);
	if (enemyUnits.empty())
	{
		GameTools::winner = "Blue Team";
	}
	else if (allyUnits.empty())
	{
		GameTools::winner = "Red Team";
	}
}
