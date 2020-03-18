#include "cGraph.h"
#include <iostream>

cNode::cNode(std::string col,glm::vec3 pos)
{
	colour = col;
	visited = false;
	gCostSoFar = FLT_MAX;
	hDistance = FLT_MAX;
	position = pos;
	parent = nullptr;
	isResource = false;
	isFinish = false;
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
			auto newNode = new cNode(color,glm::vec3(x,10,y));
			
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
			const auto currNode = mGraph[x][y];
			currNode->visited = false;
			currNode->parent = nullptr;
			currNode->gCostSoFar = FLT_MAX;
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
			result.push_back(node);
		}
	}
	
	temp_x = x+1;
	if(temp_x < this->width)
	{
		auto node = mGraph[temp_x][y];
		if(node->colour != "blk")
		{
			result.push_back(node);
		}
	}
	
	// vertical
	auto temp_y = y-1;
	if(temp_y >= 0)
	{
		auto node = mGraph[x][temp_y];
		if(node->colour != "blk")
		{
			result.push_back(node);
		}
	}
	
	temp_y = y+1;
	if(temp_y < this->height)
	{
		auto node = mGraph[x][temp_y];
		if(node->colour != "blk")
		{
			result.push_back(node);
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
				result.push_back(node);
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
				result.push_back(node);
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
				result.push_back(node);
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
				result.push_back(node);
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

int cGraph::getCost(cNode origin, cNode destiny)
{
	intPair a = {origin.position.x,origin.position.y};
	intPair b = {destiny.position.x,destiny.position.y};
	// check if diagonal
	if(a.first != b.first && a.second != b.second )
	{
		if(destiny.colour == "ylw")
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
		if(destiny.colour == "ylw")
		{
			return 20;
		}
		else
		{
			return 10;
		}
	}
}


