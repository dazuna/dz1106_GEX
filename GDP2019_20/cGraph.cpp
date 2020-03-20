#include "cGraph.h"
#include <iostream>

cNode::cNode(std::string col,glm::vec3 pos)
{
	this->colour = col;
	this->visited = false;
	this->gCostSoFar = INT_MAX;
	this->hDistance = INT_MAX;
	this->position = pos;
	this->parent = nullptr;
	this->isResource = false;
	this->isFinish = false;
}

cGraph::cGraph(BMPImage* bmpImage)
{
	this->width = bmpImage->GetImageWidth();
	this->height = bmpImage->GetImageHeight();

	this->start = bmpImage->start;
	this->finish = bmpImage->finish;
	this->resource = bmpImage->resource;

	// reserve space for the whole bmp translated to nodes.
	mGraph.resize(width);
	for(auto i = 0; i<width; i++)
	{
		mGraph[i].resize(height);
	}

	auto idx = 0;
	for(auto y = 0; y < height; y++)
	{
		for(auto x = 0; x < width; x++)
		{
			const std::string color = (bmpImage->rgbVector[idx]);
			auto newNode = new cNode(color,glm::vec3(x,y,1));
			
			// check if the node is the goal, start, or resource
			checkGoalOrResource(newNode,{x,y});
			
			mGraph[x][y] = newNode;
			idx++;
		}
	}
}

void cGraph::checkGoalOrResource(cNode* node, std::pair<int,int> pixelPos) const
{
	if(this->resource == pixelPos)	{node->isResource = true;}
	if(this->finish == pixelPos)	{node->isFinish = true;}
}

void cGraph::ResetGraph() 
{
	for(auto y = 0; y < height; y++)
	{
		for(auto x = 0; x < width; x++)
		{
			auto currNode = mGraph[x][y];
			currNode->visited = false;
			currNode->parent = nullptr;
			currNode->gCostSoFar = INT_MAX;
			currNode->hDistance = INT_MAX;
		}
	}
}

void cGraph::PrintGraph()
{
	for(auto y=height-1; y>=0; y--)
	{
		for(auto x=0; x<width; x++)
		{
			const auto currNode = mGraph[x][y];
			std::cout << currNode->colour << " ";
		}
		std::cout<<std::endl;
	}
}

cGraph::nodeVec cGraph::getParents(cNode* theNode)
{
	auto parent = theNode->parent;
	nodeVec result = {theNode};
	
	while(parent)
	{
		result.push_back(parent);
		parent = parent->parent;
	}

	std::reverse(result.begin(),result.end());
	return result;
}

cGraph::nodeVec cGraph::getNeighbors(int x, int y)
{
	nodeVec result = {};

	// ##############################    horizontal & vertical    ##############################
	
	// horizontal
	auto temp_x = x-1;	
	if(temp_x >= 0)
	{
		auto node = mGraph[temp_x][y];
		if(node->colour != "blk")
		{
			result.push_back(mGraph[temp_x][y]);
		}
	}
	
	temp_x = x+1;
	if(temp_x < this->width)
	{
		auto node = mGraph[temp_x][y];
		if(node->colour != "blk")
		{
			result.push_back(mGraph[temp_x][y]);
		}
	}
	
	// vertical
	auto temp_y = y-1;
	if(temp_y >= 0)
	{
		auto node = mGraph[x][temp_y];
		if(node->colour != "blk")
		{
			result.push_back(mGraph[x][temp_y]);
		}
	}
	
	temp_y = y+1;
	if(temp_y < this->height)
	{
		auto node = mGraph[x][temp_y];
		if(node->colour != "blk")
		{
			result.push_back(mGraph[x][temp_y]);
		}
	}
	
	// ##############################    DIAGONALS    ##############################
	
	// positive,positive
	temp_x = x+1;
	temp_y = y+1;
	if(temp_x < this->width && temp_y < this->height)
	{
		auto node = mGraph[temp_x][temp_y];
		if(node->colour != "blk")
		{
			if(mGraph[temp_x-1][temp_y]->colour != "blk" && mGraph[temp_x][temp_y-1]->colour != "blk")
			{
				result.push_back(mGraph[temp_x][temp_y]);
			}
		}
	}
	// positive,negative
	temp_x = x+1;
	temp_y = y-1;
	if(temp_x < this->width && temp_y >= 0)
	{
		auto node = mGraph[temp_x][temp_y];
		if(node->colour != "blk")
		{
			if(mGraph[temp_x-1][temp_y]->colour != "blk" && mGraph[temp_x][temp_y+1]->colour != "blk")
			{
				result.push_back(mGraph[temp_x][temp_y]);
			}
		}
	}
	// negative,positive
	temp_x = x-1;
	temp_y = y+1;
	if(temp_x >= 0 && temp_y < this->height)
	{
		auto node = mGraph[temp_x][temp_y];
		if(node->colour != "blk")
		{
			if(mGraph[temp_x+1][temp_y]->colour != "blk" && mGraph[temp_x][temp_y-1]->colour != "blk")
			{
				result.push_back(mGraph[temp_x][temp_y]);
			}
		}
	}
	// negative,negative
	temp_x = x-1;
	temp_y = y-1;
	if(temp_x >= 0 && temp_y >= 0)
	{
		auto node = mGraph[temp_x][temp_y];
		if(node->colour != "blk")
		{
			if(mGraph[temp_x+1][temp_y]->colour != "blk" && mGraph[temp_x][temp_y+1]->colour != "blk")
			{
				result.push_back(mGraph[temp_x][temp_y]);
			}
		}
	}

	return result;
}

int cGraph::getHeuristicDistance(intPair a, intPair b)
{
	// first = x, second = y
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int cGraph::getCost(cNode* origin, cNode* destin)
{
	intPair a = {int(origin->position.x),int(origin->position.y)};
	intPair b = {int(destin->position.x),int(destin->position.y)};
	// check if diagonal
	if(a.first != b.first && a.second != b.second )
	{
		if(destin->colour == "ylw")
		{
			return 28;
		}
		else
		{
			return 14;
		}
	}
	else
	{
		if(destin->colour == "ylw")
		{
			return 20;
		}
		else
		{
			return 10;
		}
	}
}

bool cGraph::isNodeInOpenList(const nodeVec& openList, cNode* child)
{
	for(auto currentNode : openList)
    {
		if (currentNode == child) { return true; }
    }
    return false;
}

cNode* cGraph::Dijkstra(cNode* rootNode)
{
	//ResetGraph();

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
        if (currNode->isResource) 
        {
            return currNode;
        }

        //Go through every child node node
    	nodeVec neighbors = getNeighbors(int(currNode->position.x),int(currNode->position.y));
        for(auto child : neighbors) 
        {    
            if(child->visited == false)
            {
                int weightSoFar = currNode->gCostSoFar + getCost(currNode,child);
                if (weightSoFar < child->gCostSoFar)
                {
                    //update node when new better path is found
                    child->gCostSoFar = weightSoFar;
                    child->parent = currNode;
                    if (!isNodeInOpenList(openList, child))
                    {
                        openList.push_back(child); //add newly discovered node to open list
                    }
                }
            }
        }
    }

    return NULL;
}

cNode* cGraph::AStar(cNode* rootNode, cNode* goal)
{
	intPair a = std::make_pair(int(rootNode->position.x),int(rootNode->position.y));
	intPair b = std::make_pair(int(goal->position.x),int(goal->position.y));
    rootNode->hDistance = getHeuristicDistance(a,b);
    rootNode->gCostSoFar = 0;

    std::vector<cNode*> closedList;
    std::vector<cNode*> openList;
    openList.push_back(rootNode);

    while (!openList.empty())
    {
        int minCost = INT_MAX;
        int minIndex = 0;
        cNode* currNode;
        //find node with the lowest cost from root node and heuristic distance from the goal node
        for (size_t i = 0; i < openList.size(); i++)
        {
            if (openList[i]->gCostSoFar + openList[i]->hDistance < minCost)
            {
                minCost = openList[i]->gCostSoFar + openList[i]->hDistance;
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

		std::cout << int(currNode->position.x) <<","<< int(currNode->position.y) << std::endl;
        currNode->visited = true;
        if (currNode->isFinish) 
        {
            return currNode;
        }

        //Go through every child node node 
    	nodeVec neighbors = getNeighbors(int(currNode->position.x),int(currNode->position.y));
        for (auto child : neighbors)
        {
            if (child->visited == false)
            {
                float weightSoFar = currNode->gCostSoFar + getCost(currNode,child);
                if (weightSoFar < child->gCostSoFar)
                {
                    child->gCostSoFar = weightSoFar;
                    child->parent = currNode;
                    if (!isNodeInOpenList(openList, child))
                    {
                    	intPair tempPair = std::make_pair(int(child->position.x),int(child->position.y));
                        child->hDistance = getHeuristicDistance(tempPair, b);
                        openList.push_back(child);
                    }
                }
            }
        }
    }

    return NULL;
}


