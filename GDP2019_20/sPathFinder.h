#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "cGameObject.h"
#include "cGraph.h"

struct sPathFinder
{
	// singleton
	static sPathFinder* thePathFinder;
	static sPathFinder* getThePathFinder();

	cGameObject* theGO, *theResource;
	cGraph::nodeVec resourcePath,finishPath;
	glm::vec3 start,resource,finish;
	float timer;
	float waitTimer;
	int index;
	std::string currentPath;

	bool init(cGraph::nodeVec rPath, cGraph::nodeVec fPath, cGraph* theGraph);
	void growToWorldSize();
	void setTheGameObject(cGameObject* gameObject);
	void setTheResource(cGameObject* gameObject);
	void update(float dt);
};

