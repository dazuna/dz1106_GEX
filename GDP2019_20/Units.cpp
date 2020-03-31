#include "Units.h"
#include "LodePNG/lodepng.h"
#include <iostream>
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"
#include "GameTools.h"
#include "GameCursor.h"

vUnits GameUnits::allyUnits, GameUnits::enemyUnits;
vUnits::iterator GameUnits::selectedAlly = GameUnits::allyUnits.begin(),
GameUnits::selectedEnemy = GameUnits::enemyUnits.begin();

GameUnit* GameUnits::getUnitByCoord(const vUnits& units, int x, int y)
{
	for (auto unit : units)
	{
		if (unit->coord_x == x && unit->coord_y == y)
		{
			return unit;
		}
	}
	return nullptr;
}

std::string GameUnits::getTypeFromColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0) return "sword";
	if (r == 0 && g == 255 && b == 0) return "archer";
	if (r == 0 && g == 0 && b == 255) return "wizard";
	return "";
}

bool GameUnits::loadUnits(vUnits& vUnits, std::string filename)
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

bool GameUnits::loadAllies(std::string filename)
{
	auto res = loadUnits(allyUnits, filename);
	if (res)
	{
		selectedAlly = allyUnits.begin();
	}
	return res;
}

bool GameUnits::loadEnemies(std::string filename)
{
	auto res = loadUnits(enemyUnits, filename);
	if (res)
	{
		selectedEnemy = enemyUnits.begin();
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
	unit->gameObj = newUnit;
	return newUnit;
}

void GameUnits::setUnitObjects()
{
	for (auto unit : allyUnits)
	{
		cGameObject* newUnit = createUnitObj(unit);
		if (newUnit)
		{
			newUnit->setAT(glm::vec3(1, 0, 0));
			newUnit->textures[0] = "Blue.png";
		}
	}
	
	for (auto unit : enemyUnits)
	{
		cGameObject* newUnit = createUnitObj(unit);
		if (newUnit)
		{
			newUnit->setAT(glm::vec3(-1, 0, 0));
			newUnit->textures[0] = "Red.png";
		}
	}
}

void GameUnits::nextAlly()
{
	(*selectedAlly)->gameObj->textures[0] = "Blue.png";
	selectedAlly++;
	if (selectedAlly == allyUnits.end())
	{
		selectedAlly = allyUnits.begin();
	}
	(*selectedAlly)->gameObj->textures[0] = "Yellow.png";
	GameCursor::setPosition((*selectedAlly)->coord_x, (*selectedAlly)->coord_y);
}

void GameUnits::previousAlly()
{
	(*selectedAlly)->gameObj->textures[0] = "Blue.png";
	if (selectedAlly == allyUnits.begin())
	{
		selectedAlly = allyUnits.end();
	}
	selectedAlly--;
	(*selectedAlly)->gameObj->textures[0] = "Yellow.png";
	GameCursor::setPosition((*selectedAlly)->coord_x, (*selectedAlly)->coord_y);
}
