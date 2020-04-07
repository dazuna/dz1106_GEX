#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"

struct GamePathFinding 
{
	struct cNode
	{
		bool visited = false;
		int gCostSoFar = INT_MAX;
		cNode* parent = nullptr;
		int coord_x, coord_y;
		cNode(int x, int y);
		std::string terrainType;
	};	
	typedef std::vector<cNode*> nodeVec;

	GamePathFinding();
	std::vector<nodeVec> mGraph;

    void ResetGraph();
    void PrintGraph();
	nodeVec getRouteTo(cNode* goal);

	// Algorithm Tools
	/*
	 * Similar to GameTools::isCoordWalkable, but player's units
	 * are considered "walkable". We want to get to them after all
	 */
	bool isPossibleNeighbor(int x, int y);
	nodeVec getNeighbors(int x, int y);
	int getCost(cNode* b);
	bool isNodeInOpenList(const nodeVec& openList, cNode* child);

	// Algorithms
	cNode* Dijkstra(int x, int y);
};

