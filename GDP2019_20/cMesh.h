#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>		// "smart array"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// This structure matches the file, so that our life is
//   simpler and happier, and we all get along.
struct sPlyVertexXYZ_N
{
	float x, y, z;
	float nx, ny, nz;		// NOW with NORMALS!!
};

struct sPlyTriangle
{
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};

class cMesh
{
public:
	cMesh() {};			// Empty c'tor
	~cMesh() {};		// Empty d'tor
	// Store the vertices
	std::vector<sPlyVertexXYZ_N> vecVertices;

	// Store the triangles
	std::vector<sPlyTriangle> vecTriangles;

};

#endif 
