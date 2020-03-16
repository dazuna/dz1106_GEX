#pragma once
#include <map>
#include <string>
#include <mutex>
#include "cMesh.h"

struct cMeshMap
{
	// Singleton
	static cMeshMap* theMeshMap;
	static cMeshMap* getTheMeshMap();

	cMeshMap(){meshMap_ = {};}
	std::map<std::string, cMesh*> meshMap_;
	// Mesh Map Mutex
	std::mutex mm_mutex;

	void addMesh(std::string name, cMesh* mesh);
	cMesh* getMesh(std::string name);
	bool findMesh(std::string name);
};
