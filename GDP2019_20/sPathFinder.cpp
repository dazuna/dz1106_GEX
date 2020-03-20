#include "sPathFinder.h"

sPathFinder* sPathFinder::thePathFinder = new sPathFinder();

sPathFinder* sPathFinder::getThePathFinder()
{
	return thePathFinder;
}

bool sPathFinder::init(cGraph::nodeVec rPath, cGraph::nodeVec fPath, cGraph* theGraph)
{
	theGO = nullptr;
	theResource = nullptr;
	resourcePath = rPath;
	finishPath = fPath;
	start = glm::vec3(theGraph->start.first,theGraph->start.second,1);
	resource = glm::vec3(theGraph->resource.first,theGraph->resource.second,1);
	finish = glm::vec3(theGraph->finish.first,theGraph->finish.second,1);
	timer = 0.f;
	waitTimer = 0.f;
	index = 0;
	currentPath = "resource";
	//growToWorldSize();
	return true;
}

void sPathFinder::growToWorldSize()
{
	const auto plus5 = glm::vec3(5.f,5.f,0.f);
	start		= (start*10.f)		+plus5;
	resource	= (resource*10.f)	+plus5;
	finish		= (finish*10.f)		+plus5;
	std::cout << "resourcePath: " << std::endl;
	for(auto* node : resourcePath)
	{
		std::cout << node->position.x << "," << node->position.y << "," << node->position.z << " | ";
		node->position = (node->position*10.f)+plus5;
		std::cout << node->position.x << "," << node->position.y << "," << node->position.z << std::endl;
	}
	std::cout << "resourcePath: " << std::endl;
	for(auto* node : finishPath)
	{
		std::cout << node->position.x << "," << node->position.y << "," << node->position.z << " | ";
		node->position = (node->position*10.f)+plus5;
		std::cout << node->position.x << "," << node->position.y << "," << node->position.z << std::endl;
	}
}

void sPathFinder::setTheGameObject(cGameObject* gameObject)
{
	const auto plus5 = glm::vec3(5.f,5.f,0.f);
	theGO = gameObject;
	theGO->positionXYZ = (this->start*10.f)+plus5;
}

void sPathFinder::setTheResource(cGameObject* gameObject)
{
	const auto plus5 = glm::vec3(5.f,5.f,0.f);
	theResource = gameObject;
	theResource->positionXYZ = (this->resource*10.f)+plus5;
}

void sPathFinder::update(float dt)
{
	const auto plus5 = glm::vec3(5.f,5.f,0.f);
	timer += dt;
	if(timer>0.8f)
	{
		timer = 0.f;
		if(currentPath == "resource")
		{
			index++;
			if(index < resourcePath.size())
			{
				theGO->positionXYZ = (resourcePath[index]->position*10.f)+plus5;
			}
			else
			{
				index = 0;
				currentPath = "wait";
			}
		}
		if(currentPath == "wait")
		{
			if(waitTimer<7.f)
			{
				waitTimer++;
			}
			else
			{
				waitTimer = 0.f;
				currentPath = "finish";
			}
		}
		if(currentPath == "finish")
		{
			index++;
			if(index < finishPath.size())
			{
				theGO->positionXYZ = (finishPath[index]->position*10.f)+plus5;
				if(theResource)
				{
					theResource->positionXYZ = theGO->positionXYZ+glm::vec3(2,2,0);
				}
			}
			else
			{
				currentPath = "wait2";
			}
		}
		if(currentPath == "wait2")
		{
			if(waitTimer<2.f)
			{
				waitTimer++;
			}
			else
			{
				waitTimer = 0.f;
				if(theResource)
				{
					theResource->positionXYZ = glm::vec3(0);
				}
				currentPath = "finish";
			}
		}
	}
}

