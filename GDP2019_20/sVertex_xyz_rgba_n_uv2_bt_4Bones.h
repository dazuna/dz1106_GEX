#pragma once

static const unsigned int NUMBEROFBONES = 4;
struct sVertex_xyz_rgba_n_uv2_bt_4Bones
{
	//sVertex_xyz_rgba_n_uv2_bt_4Bones();
	//sVertex_xyz_rgba_n_uv2_bt_4Bones::sVertex_xyz_rgba_n_uv2_bt_4Bones():
	sVertex_xyz_rgba_n_uv2_bt_4Bones() :
	x(0.0f), y(0.0f), z(0.0f), w(1.0f),
	r(0.0f), g(0.0f), b(0.0f), a(1.0f),		// Note alpha is 1.0
	nx(0.0f), ny(0.0f), nz(0.0f), nw(1.0f),
	u0(0.0f), v0(0.0f), u1(0.0f), v1(0.0f),
	tx(0.0f), ty(0.0f), tz(0.0f), tw(1.0f),
	bx(0.0f), by(0.0f), bz(0.0f), bw(1.0f)
	{
		//#ifdef _DEBUG
		memset(this->boneID, 0, sizeof(unsigned int) * NUMBEROFBONES);
		memset(this->boneWeights, 0, sizeof(float) * NUMBEROFBONES);

		//this->boneID[0] = 0;
		//this->boneID[1] = 0;
		//this->boneID[2] = 0;
		//this->boneID[3] = 0;

		return;
	}
	// constructor

	float x, y, z, w;			// w coordinate	
	float r, g, b, a;	// a = alpha (transparency)
	float nx, ny, nz, nw;
	float u0, v0, u1, v1;

	// For bump mapping
	float tx, ty, tz, tw;	// tangent				// 
	float bx, by, bz, bw;	// bi-normal			// 

	// For skinned mesh
	// For the 4 bone skinned mesh information
	float boneID[NUMBEROFBONES]; 		// New		// 
	float boneWeights[NUMBEROFBONES];	// New		// 
};
