#include "Terrain.h"
#include "LodePNG/lodepng.h"
#include <iostream>
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"
#include "GameTools.h"

matTerrain Terrain::terrainGrid;
unsigned Terrain::width = 0,
	Terrain::height = 0;

std::string Terrain::getTypeFromColor(unsigned char r, unsigned char g, unsigned char b)
{
	//if (r == 0 && g == 0 && b == 255) return "water";
	//if (r == 255 && g == 255 && b == 255) return "wht";
	//if (r == 255 && g == 255 && b == 0) return "ylw";
	if (r == 0 && g == 0 && b == 0) return "wall";
	if (r == 0 && g == 255 && b == 0) return "vehicleSpawn";
	if (r == 255 && g == 255 && b == 255) return "ground";
	return "xxx";
}

bool Terrain::loadTerrain(std::string filename)
{
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error)
	{
		std::cout << "error loading the terrain map: " << error << ": " << lodepng_error_text(error) << filename << std::endl;
		return false;
	}

	terrainGrid.resize(width);
	for (auto i = 0; i < width; i++)
	{
		terrainGrid[i].resize(height);
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
			terrainGrid[x][height - y - 1] = getTypeFromColor(red, green, blue);
		}
	}
	int i = 0;
	i++;
	return true;
}

void Terrain::setTerrainObjects()
{
	for (auto i = 0; i < width; i++)
	{
		for (auto j = 0; j < height; j++)
		{
			cGameObject* newTerrain;
			if (terrainGrid[i][j] == "ground")
			{
				if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
				{
					std::cout << "No groundBlock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			} else if (terrainGrid[i][j] == "vehicleSpawn")
			{
				if (!tools::pFindObjectByFriendlyNameMap("forestBlock"))
				{
					std::cout << "No forestBlock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["forestBlock"]);
			} else if (terrainGrid[i][j] == "wall")
			{
				if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
				{
					std::cout << "No groundBlock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			} else continue;
			newTerrain->isVisible = true;
			// The cube model is 2 units long
			newTerrain->scale = GameTools::worldScale / 100;
			newTerrain->positionXYZ = GameTools::coordToWorldPos(i, j);
			// put the top of the terrain on the xz plane
			newTerrain->positionXYZ.y = -GameTools::worldScale / 2;
			//::g_map_GameObjects[newTerrain->friendlyName] = newTerrain;
			auto theSceneManager = cSceneManager::getTheSceneManager();
			theSceneManager->scenesVector[0]->addGameObject(newTerrain);

			// Add some rocks on top of the wall block
			if (terrainGrid[i][j] == "wall")
			{
				if (!tools::pFindObjectByFriendlyNameMap("mossyRock"))
				{
					std::cout << "No mossyRock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["mossyRock"]);
				newTerrain->scale *= (GameTools::worldScale*2);
				newTerrain->isVisible = true;
				newTerrain->positionXYZ = GameTools::coordToWorldPos(i, j);
				theSceneManager->scenesVector[0]->addGameObject(newTerrain);
			}

			if(terrainGrid[i][j] == "vehicleSpawn")
			{
				if (!tools::pFindObjectByFriendlyNameMap("vehicle"))
				{
					std::cout << "No vehicle model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["vehicle"]);
				newTerrain->setAT(glm::vec3(0,0,-1));
				newTerrain->scale *= (GameTools::worldScale);
				newTerrain->isVisible = true;
				newTerrain->positionXYZ = GameTools::coordToWorldPos(i, j);
				newTerrain->positionXYZ += glm::vec3(0,newTerrain->scale,0);
				theSceneManager->scenesVector[0]->addGameObject(newTerrain);
				::g_map_GameObjects.insert({newTerrain->friendlyName,newTerrain});
				GameTools::vehicles.push_back(cVehicle(newTerrain));
			}
		}
	}
}

bool Terrain::isTerrainWalkable(std::string type)
{
	auto res = type != "wall" && type != "water";
	//std::cout << type << " is " << (res ? "" : "not") << " walkable" << std::endl;
	return res;
}
