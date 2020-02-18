#pragma once

#include <string>
#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
//	sVertex* pVertices;	//  = 0;
	sVertex_xyz_rgba_n_uv2_bt_4Bones* pVertices;
	// The index buffer (CPU side)
	unsigned int* pIndices;

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	//	scale = 5.0/maxExtent;		-> 5x5x5
	float maxExtent;
	float extentX, extentY, extentZ;

	void CalcExtents(void);
};
