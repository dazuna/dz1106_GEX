#ifndef _cModelLoader_HG_2019_09_09_
#define _cModelLoader_HG_2019_09_09_

#include <string>	

#include "cMesh.h"

class cModelLoader
{
public:
	cModelLoader();			// constructor
	~cModelLoader();		// destructor

	// Takes the filename to load
	// Returns by ref the mesh
	bool LoadPlyModel(std::string filename, cMesh &theMesh);
	// bool LoadPlyModelNoTextures(std::string filename, cMesh& theMesh);

	bool LoadModel_Assimp(std::string filename, cMesh& theMesh, std::string& errors);

};

#endif //_cModelLoader_HG_2019_09_09_

