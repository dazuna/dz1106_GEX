#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "zBMPLoader/BMPImage.h"

struct cNode 
{
	// red,grn,blu,wht,ylw,blk
    std::string colour;
    bool visited;
    int gCostSoFar;
    int hDistance;
    glm::vec3 position;
    cNode* parent;
	bool isResource;
	bool isFinish;
	cNode(std::string col,glm::vec3 pos);
};

struct cGraph 
{
	typedef std::vector<cNode*> nodeVec;
    typedef std::pair<int,int> intPair;
	
	std::vector<nodeVec> mGraph;
    int width, height;
	intPair start,finish,resource;

    cGraph(BMPImage* bmpImage);
	void checkGoalOrResource(cNode* node,
        std::pair<int,int> pixelPos) const;

    void ResetGraph();
    void PrintGraph();
	nodeVec getParents(cNode* theNode);

	// Algorithm Tools
	nodeVec getNeighbors(int x, int y);
	int getHeuristicDistance(intPair a, intPair b);
	int getCost(cNode* a, cNode* b);
	bool isNodeInOpenList(const nodeVec& openList, cNode* child);

	// Algorithms
	cNode* Dijkstra(cNode* rootNode);
	cNode* AStar(cNode* rootNode, cNode* goal);
	
};

