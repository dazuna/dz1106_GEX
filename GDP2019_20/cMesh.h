#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>		// "smart array"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

// This structure matches the file, so that our life is
//   simpler and happier, and we all get along.
struct sPlyVertexXYZ_N
{
	float x, y, z;
	float nx, ny, nz;		// NOW with NORMALS!!
};
struct sPlyVertexXYZ_N_UV
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;				// NOW with TEXTURE COORDINATES!!!
};
struct sPlyTriangle
{
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};
struct AABBTriangle
{
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec3 n;
};

struct cMesh
{
	cMesh() : status("not_loaded") {};			// Empty c'tor
	~cMesh() {};		// Empty d'tor
	// Store the vertices
	std::vector<sPlyVertexXYZ_N_UV> vecVertices;
	// Store the triangles
	std::vector<sPlyTriangle> vecTriangles;
	// Store elegant
	std::vector<AABBTriangle> vecABTriangles;
	// Status of the Mesh
	std::string status;
};

#endif 
