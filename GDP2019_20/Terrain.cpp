#include "Terrain.h"
#include "LodePNG/lodepng.h"
#include <iostream>
#include "util/tools.h"
#include "SceneManager/cSceneManager.h"
#include "GameTools.h"
#include "InstanceRenderer.h"

matTerrain Terrain::terrainGrid;
unsigned Terrain::width = 0,
	Terrain::height = 0;

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
	// Initialize "blocks" instance renderer
	if(InstanceRenderer::mapIR.find("blocks") == InstanceRenderer::mapIR.end())
	{
		std::cout << "couldn't find the \"blocks\" instance renderer" << std::endl;
		std::cout << "creating \"blocks\" instance renderer..." << std::endl;
		InstanceRenderer::mapIR.insert({"blocks",new InstanceRenderer()});
	}
	auto blocksIR = InstanceRenderer::mapIR.at("blocks");
	if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
	{
		std::cout << "No groundBlock model!!" << std::endl;
		return;;
	}
	blocksIR->gameObj = ::g_map_GameObjects.at("groundBlock");
	
	// Initialize "trees" instance renderer
	if(InstanceRenderer::mapIR.find("trees") == InstanceRenderer::mapIR.end())
	{
		std::cout << "couldn't find the \"trees\" instance renderer" << std::endl;
		std::cout << "creating \"trees\" instance renderer..." << std::endl;
		InstanceRenderer::mapIR.insert({"trees",new InstanceRenderer()});
	}
	auto treesIR = InstanceRenderer::mapIR.at("trees");
	if (!tools::pFindObjectByFriendlyNameMap("tree"))
	{
		std::cout << "No tree model!!" << std::endl;
		return;;
	}
	treesIR->gameObj = ::g_map_GameObjects.at("tree");

	// Initialize "trees" instance renderer
	if(InstanceRenderer::mapIR.find("rocks") == InstanceRenderer::mapIR.end())
	{
		std::cout << "couldn't find the \"rocks\" instance renderer" << std::endl;
		std::cout << "creating \"rocks\" instance renderer..." << std::endl;
		InstanceRenderer::mapIR.insert({"rocks",new InstanceRenderer()});
	}
	auto rocksIR = InstanceRenderer::mapIR.at("rocks");
	if (!tools::pFindObjectByFriendlyNameMap("mossyRock"))
	{
		std::cout << "No rock model!!" << std::endl;
		return;;
	}
	rocksIR->gameObj = ::g_map_GameObjects.at("mossyRock");
	
	for (auto i = 0; i < width; i++)
	{
		for (auto j = 0; j < height; j++)
		{
			cGameObject* newTerrain;
			//if (terrainGrid[i][j] == "ground")
			//{
			//	if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
			//	{
			//		std::cout << "No groundBlock model!!" << std::endl;
			//		continue;
			//	}
			//	newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			//} else if (terrainGrid[i][j] == "tree")
			//{
			//	if (!tools::pFindObjectByFriendlyNameMap("forestBlock"))
			//	{
			//		std::cout << "No forestBlock model!!" << std::endl;
			//		continue;
			//	}
			//	newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			//} else if (terrainGrid[i][j] == "wall")
			//{
			//	if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
			//	{
			//		std::cout << "No groundBlock model!!" << std::endl;
			//		continue;
			//	}
			//	newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			//} else continue;			
			if (!tools::pFindObjectByFriendlyNameMap("groundBlock"))
			{
				std::cout << "No groundBlock model!!" << std::endl;
				continue;
			}		
			newTerrain = new cGameObject(::g_map_GameObjects["groundBlock"]);
			newTerrain->isVisible = true;
			// The cube model is 2 units long
			newTerrain->scale = GameTools::worldScale / 100;
			newTerrain->positionXYZ = GameTools::coordToWorldPos(i, j);
			// put the top of the terrain on the xz plane
			newTerrain->positionXYZ.y = -GameTools::worldScale / 2;
			//::g_map_GameObjects[newTerrain->friendlyName] = newTerrain

			// add to matrices vec
			blocksIR->vecWMs.push_back(tools::calculateWorldMatrix(newTerrain));
			
			auto theSceneManager = cSceneManager::getTheSceneManager();
			//theSceneManager->scenesVector[0]->addGameObject(newTerrain);

			// Add some rocks on top of the wall block
			if (terrainGrid[i][j] == "wall")
			{
				if (!tools::pFindObjectByFriendlyNameMap("mossyRock"))
				{
					std::cout << "No mossyRock model!!" << std::endl;
					continue;
				}
				newTerrain = new cGameObject(::g_map_GameObjects["mossyRock"]);
				newTerrain->scale *= GameTools::worldScale;
				newTerrain->isVisible = true;
				newTerrain->positionXYZ = GameTools::coordToWorldPos(i, j);				
				rocksIR->vecWMs.push_back(tools::calculateWorldMatrix(newTerrain));
				//theSceneManager->scenesVector[0]->addGameObject(newTerrain);
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
				tree->scale = GameTools::worldScale;
				tree->positionXYZ = GameTools::coordToWorldPos(i, j);

				treesIR->vecWMs.push_back(tools::calculateWorldMatrix(tree));
			}
		}
	}
	for(std::pair<std::string,InstanceRenderer*> insRend : InstanceRenderer::mapIR)
	{
		//blocksIR->setVAOVariables();
		if(!insRend.second->vecWMs.empty())
		{
			insRend.second->setVAOVariables();
		}
	}
}

bool Terrain::isTerrainWalkable(std::string type)
{
	auto res = type != "wall" && type != "water";
	//std::cout << type << " is " << (res ? "" : "not") << " walkable" << std::endl;
	return res;
}
