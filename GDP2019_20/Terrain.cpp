#include "Terrain.h"
#include "LodePNG/lodepng.h"
#include <iostream>
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"

matTerrain Terrain::terrainGrid;
unsigned Terrain::width = 0,
	Terrain::height = 0;
float Terrain::worldScale = 10.0f;

std::string Terrain::getTypeFromColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0) return "tree";
	if (r == 0 && g == 255 && b == 0) return "ground";
	if (r == 0 && g == 0 && b == 255) return "water";
	//if (r == 255 && g == 255 && b == 255) return "wht";
	//if (r == 255 && g == 255 && b == 0) return "ylw";
	if (r == 0 && g == 0 && b == 0) return "wall";
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
			} else if (terrainGrid[i][j] == "tree")
			{
				if (!tools::pFindObjectByFriendlyNameMap("forestBlock"))
				{
					std::cout << "No forestBlock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["forestBlock"]);
			} else if (terrainGrid[i][j] == "wall")
			{
				if (!tools::pFindObjectByFriendlyNameMap("wallBlock"))
				{
					std::cout << "No wallBlock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["wallBlock"]);
			} else continue;
			newTerrain->isVisible = true;
			// The cube model is 2 units long
			newTerrain->scale = worldScale / 100;
			newTerrain->positionXYZ = coordToWorldPos(i, j);
			// put the top of the terrain on the xz plane
			newTerrain->positionXYZ.y = -worldScale / 2;
			//::g_map_GameObjects[newTerrain->friendlyName] = newTerrain;
			auto theSceneManager = cSceneManager::getTheSceneManager();
			theSceneManager->scenesVector[0]->addGameObject(newTerrain);

			// Add a wall block on top of the ground
			// maybe we could change this to a more "proper" prop
			if (terrainGrid[i][j] == "wall")
			{
				newTerrain = new cGameObject(newTerrain);
				newTerrain->positionXYZ.y += worldScale;
				//::g_map_GameObjects[newTerrain->friendlyName] = newTerrain;
				theSceneManager->scenesVector[0]->addGameObject(newTerrain);
			}
			
			// Add a tree to the top of forest blocks
			if (terrainGrid[i][j] == "tree")
			{
				if (!tools::pFindObjectByFriendlyNameMap("tree"))
				{
					std::cout << "No tree base object!!" << std::endl;
					continue;
				}
				auto tree = new cGameObject(::g_map_GameObjects["tree"]);
				tree->isVisible = true;
				tree->scale = worldScale;
				tree->positionXYZ = coordToWorldPos(i, j);
				//::g_map_GameObjects[tree->friendlyName] = tree;
				theSceneManager->scenesVector[0]->addGameObject(tree);
			}
		}
	}
}

glm::vec3 Terrain::coordToWorldPos(int i, int j)
{
	return glm::vec3(i * worldScale, 0, j * worldScale);
}
