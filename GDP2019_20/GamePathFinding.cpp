#include "GamePathFinding.h"
#include <iostream>
#include "Terrain.h"
#include "GameTools.h"
#include "GameArmies.h"

GamePathFinding::cNode::cNode(int x, int y) :
	coord_x (x),
	coord_y (y)
{
	terrainType = Terrain::terrainGrid[x][y];
}

GamePathFinding::GamePathFinding()
{
	// reserve space for the whole bmp translated to nodes.
	mGraph.resize(Terrain::width);
	for(auto x = 0; x < Terrain::width; x++)
	{
		mGraph[x].resize(Terrain::height);
		for(auto y = 0; y < Terrain::height; y++)
		{
			auto newNode = new cNode(x,y);
			mGraph[x][y] = newNode;
		}
	}
}

void GamePathFinding::ResetGraph() 
{
	for(auto columns : mGraph)
	{
		for(auto node : columns)
		{
			node->visited = false;
			node->parent = nullptr;
			node->gCostSoFar = INT_MAX;
		}
	}
}

void GamePathFinding::PrintGraph()
{
	for (auto columns : mGraph)
	{
		for (auto node : columns)
		{
			std::cout << "TODO: print node" << " ";
		}
		std::cout<<std::endl;
	}
}

GamePathFinding::nodeVec GamePathFinding::getRouteTo(cNode* goal)
{
	auto parent = goal->parent;
	nodeVec result;
	
	while(parent)
	{
		result.push_back(parent);
		parent = parent->parent;
	}

	std::reverse(result.begin(),result.end());
	return result;
}


bool GamePathFinding::isPossibleNeighbor(int x, int y)
{
	// The coord is inside the board
	if (!GameTools::validCoord(x, y)) return false;
	auto terrainType = Terrain::terrainGrid[x][y];
	// the terrain is walkable
	if (!Terrain::isTerrainWalkable(terrainType)) return false;
	// does the space does not contain an CPU unit
	if (GameArmies::getUnitByCoord(GameArmies::enemyUnits, x, y)) return false;
	return true;
}

GamePathFinding::nodeVec GamePathFinding::getNeighbors(int x, int y)
{
	nodeVec result = {};

	// ##############################    horizontal & vertical    ##############################
	
	// horizontal
	auto temp_x = x-1;	
	if(isPossibleNeighbor(temp_x, y))
	{
		result.push_back(mGraph[temp_x][y]);
	}
	temp_x = x+1;
	if(isPossibleNeighbor(temp_x, y))
	{
		result.push_back(mGraph[temp_x][y]);
	}
	
	// vertical
	auto temp_y = y-1;
	if(isPossibleNeighbor(x, temp_y))
	{
		result.push_back(mGraph[x][temp_y]);
	}
	
	temp_y = y+1;
	if(isPossibleNeighbor(x, temp_y))
	{
		result.push_back(mGraph[x][temp_y]);
	}

	return result;
}

int GamePathFinding::getCost(cNode* destin)
{
	if(destin->terrainType == "tree")
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

bool GamePathFinding::isNodeInOpenList(const nodeVec& openList, cNode* child)
{
	for(auto currentNode : openList)
    {
		if (currentNode == child) { return true; }
    }
    return false;
}

GamePathFinding::cNode* GamePathFinding::Dijkstra(int x, int y)
{
	ResetGraph();

	auto rootNode = mGraph[x][y];
    rootNode->visited = true;
    rootNode->gCostSoFar = 0;
	std::vector<cNode*> closedList;
	std::vector<cNode*> openList;
    openList.push_back(rootNode);

    while (!openList.empty())
    {
        int minCost = INT_MAX;
        int minIndex = 0;
        cNode* currNode;
        //find node with the lowest cost from root node
        for (size_t i = 0; i < openList.size(); i++)
        {
            if (openList[i]->gCostSoFar < minCost) 
            {
                minCost = openList[i]->gCostSoFar;
                minIndex = i;
            }
        }

        //remove current node from open list and add to closed list
        currNode = openList[minIndex];
        for (auto iter = openList.begin(); iter != openList.end(); ++iter)
        {
            if (*iter == currNode)
            {
                openList.erase(iter);
                break;
            }
        }
        closedList.push_back(currNode);

        //std::cout << int(currNode->position.x) <<","<< int(currNode->position.y) << std::endl;
        currNode->visited = true;
        auto playerUnit = GameArmies::getUnitByCoord(
            GameArmies::allyUnits,
            currNode->coord_x,
            currNode->coord_y
        );
        if (playerUnit) 
        {
            return currNode;
        }

        //Go through every child node node
    	nodeVec neighbors = getNeighbors(int(currNode->coord_x),int(currNode->coord_y));
        for(auto neighbor : neighbors) 
        {    
            if(neighbor->visited == false)
            {
                int weightSoFar = currNode->gCostSoFar + getCost(neighbor);
                if (weightSoFar < neighbor->gCostSoFar)
                {
                    //update node when new better path is found
                    neighbor->gCostSoFar = weightSoFar;
                    neighbor->parent = currNode;
                    if (!isNodeInOpenList(openList, neighbor))
                    {
                        openList.push_back(neighbor); //add newly discovered node to open list
                    }
                }
            }
        }
    }

    return nullptr;
}
