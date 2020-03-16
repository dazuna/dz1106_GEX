#include "cMeshMap.h"

cMeshMap* cMeshMap::theMeshMap = new cMeshMap();

cMeshMap* cMeshMap::getTheMeshMap()
{
	return theMeshMap;
}

void cMeshMap::addMesh(std::string name, cMesh* mesh)
{
	mm_mutex.lock();
	meshMap_.insert({name,mesh});
	mm_mutex.unlock();
}

cMesh* cMeshMap::getMesh(std::string name)
{
	cMesh* tempMesh;
	mm_mutex.lock();
	tempMesh = meshMap_.at(name);
	mm_mutex.unlock();
	return tempMesh;
}

bool cMeshMap::findMesh(std::string name)
{
	mm_mutex.lock();
	const auto exists = (meshMap_.find(name) != meshMap_.end());
	mm_mutex.unlock();
	return exists;
}
