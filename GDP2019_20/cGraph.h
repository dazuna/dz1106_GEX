#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "zBMPLoader/BMPImage.h"

struct cNode 
{
    std::string colour;
    bool visited;
    float gCostSoFar;
    float hDistance;
    glm::vec3 position;
    cNode* parent;
	bool isResource;
	bool isFinish;
	cNode(std::string col,glm::vec3 pos);
};

struct cGraph 
{
	typedef std::vector<cNode*> nodeVec;
	std::vector<nodeVec> mGraph;
    int width, height;
	std::pair<int,int> start,finish,resource;

    cGraph(BMPImage* bmpImage);
	void checkGoalOrResource(cNode* node,
        std::pair<int,int> pixelPos) const;

    void ResetGraph();
    void PrintGraph();

	nodeVec getNeighbors(int x, int y);
	
};

