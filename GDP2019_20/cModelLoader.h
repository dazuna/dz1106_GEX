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
	bool LoadPlyModel(std::string filename,
		              cMesh &theMesh);		// Note the & (by reference)

};

#endif //_cModelLoader_HG_2019_09_09_

