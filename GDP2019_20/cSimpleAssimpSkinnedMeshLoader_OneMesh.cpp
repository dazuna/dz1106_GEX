#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <glad/glad.h>

#include <sstream>


//#define OUTPUT_STUFF 1


glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat)
{
	return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,	
					mat.a2, mat.b2, mat.c2, mat.d2,
					mat.a3, mat.b3, mat.c3, mat.d3,
					mat.a4, mat.b4, mat.c4, mat.d4);
}


cSimpleAssimpSkinnedMesh::cSimpleAssimpSkinnedMesh(void)
{
	this->pScene = 0;
	this->mNumBones = 0;

	this->m_numberOfVertices = 0;
	this->m_numberOfIndices = 0;
	this->m_numberOfTriangles = 0;

	//this->m_VAO_ID = 0;				// Vertex Array Object
	//this->m_VBO_ID = 0;				// Vertex buffer object 
	//this->m_indexBuf_ID = 0;		// Index buffer referring to VBO
	//this->m_bVAO_created = false;
	//this->m_shaderIDMatchingVAO = 0;

	return;
}



cSimpleAssimpSkinnedMesh::~cSimpleAssimpSkinnedMesh(void)
{
	this->ShutErDown();

	return;
}

void cSimpleAssimpSkinnedMesh::ShutErDown(void)
{
	//if ( this->m_pVertexData )
	//{
	//	delete [] this->m_pVertexData;
	//}
	// TODO: Delete the OpenGL buffers, too??

	return;
}

bool cSimpleAssimpSkinnedMesh::LoadMeshFromFile( const std::string &friendlyName, 
												 const std::string &filename )		// mesh we draw
{
	unsigned int Flags = aiProcess_Triangulate | 
		                 aiProcess_OptimizeMeshes | 
		                 aiProcess_OptimizeGraph | 
		                 aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace;

	this->pScene = this->mImporter.ReadFile(filename.c_str(), Flags);

	//aiMesh* pM0 = this->pScene->mMeshes[0];
	////aiMesh* pM1 = this->mpScene->mMeshes[1];
	////aiMesh* pM2 = this->mpScene->mMeshes[2];
	////aiMesh* pM3 = this->mpScene->mMeshes[3];
	if ( this->pScene )
	{
		this->fileName = filename;
		// Assume the friendlyName is the same as the file, for now
		this->friendlyName = friendlyName;

		this->mGlobalInverseTransformation = AIMatrixToGLMMatrix( pScene->mRootNode->mTransformation );
		this->mGlobalInverseTransformation = glm::inverse(this->mGlobalInverseTransformation);
		
		// Calcualte all the bone things
		if ( ! this->Initialize() )
		{	// There was an issue doing this calculation
			return false;
		}
	}//if ( this->pScene )

	return true;
}


													// Looks in the animation map and returns the total time
float cSimpleAssimpSkinnedMesh::FindAnimationTotalTime(std::string animationName)
{
	//std::map< std::string /*animationfile*/,
	//	const aiScene* >::iterator itAnimation = this->mapAnimationNameTo_pScene.find(animationName);
	std::map< std::string /*animation FRIENDLY name*/,
		        sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(animationName);
	
	// Found it? 
	if ( itAnimation == this->mapAnimationFriendlyNameTo_pScene.end() )
	{	// Nope.
		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mDuration;	
}


bool cSimpleAssimpSkinnedMesh::LoadMeshAnimation( const std::string &friendlyName,
												  const std::string &filename )	// Only want animations
{
	// Already loaded this? 
	std::map< std::string /*animation FRIENDLY name*/,
		        sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(friendlyName);
	
	// Found it? 
	if ( itAnimation != this->mapAnimationFriendlyNameTo_pScene.end() )
	{	// Yup. So we already loaded it.
		return false;
	}


//	std::map< std::string /*animationfile*/,
//		      const aiScene* > mapAnimationNameTo_pScene;		// Animations

	unsigned int Flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices;

	Assimp::Importer* pImporter = new Assimp::Importer();
//	const aiScene* pAniScene = pImporter->ReadFile(filename.c_str(), Flags);
	sAnimationInfo animInfo;
	animInfo.friendlyName = friendlyName;
	animInfo.fileName = filename;
	animInfo.pAIScene = pImporter->ReadFile(animInfo.fileName.c_str(), Flags);

	if ( ! animInfo.pAIScene )
	{
		return false;
	}
	// Animation scene is loaded (we assume)
	// Add it to the map
	//this->mapAnimationNameTo_pScene[filename] = animInfo;
	this->mapAnimationFriendlyNameTo_pScene[animInfo.friendlyName] = animInfo;

	return true;
}

// Added to assist with the forward kinematics (id and bone names)
void cSimpleAssimpSkinnedMesh::GetListOfBoneIDandNames( std::vector<std::string> &vecBoneNames )
{
	// Go through the list of channels and return the id and name
	for ( unsigned int chanID = 0;
		  chanID != this->pScene->mAnimations[0]->mNumChannels;
		  chanID++ )
	{
		std::string boneName;
		boneName.append( this->pScene->mAnimations[0]->mChannels[chanID]->mNodeName.data );
		vecBoneNames.push_back( boneName );
	}
	return;
}



	//const aiScene* pScene;		// "pretty" mesh we update and draw
	//Assimp::Importer mImporter;

	//std::map< std::string /*animationfile*/,
	//	      const aiScene* > mapAnimationNameTo_pScene;		// Animations


bool cSimpleAssimpSkinnedMesh::Initialize(void)
{
	this->m_numberOfVertices = this->pScene->mMeshes[0]->mNumVertices;

	// This is the vertex information for JUST the bone stuff
	this->vecVertexBoneData.resize(this->m_numberOfVertices);

	this->LoadBones( this->pScene->mMeshes[0], this->vecVertexBoneData );

	return true;
}


void cSimpleAssimpSkinnedMesh::sVertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for ( unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++ )
	{
		if ( this->weights[Index] == 0.0f )
		{
			this->ids[Index]		= (float)BoneID;
			this->weights[Index]	= Weight;
			return;
		}
	}
}

// In the original code, these vectors are being passed out into the "character" object.
// It's unclear what the Globals matrices are actually for...
//void cSimpleAssimpSkinnedMesh::BoneTransform( float TimeInSeconds, 
//                                              std::vector<glm::mat4> &FinalTransformation, 
//								              std::vector<glm::mat4> &Globals, 
//								              std::vector<glm::mat4> &Offsets)
void cSimpleAssimpSkinnedMesh::BoneTransform( float TimeInSeconds, 
											  std::string animationName,		// Now we can pick the animation
                                              std::vector<glm::mat4> &FinalTransformation, 
								              std::vector<glm::mat4> &Globals, 
								              std::vector<glm::mat4> &Offsets)
{
	glm::mat4 Identity(1.0f);

	auto x = this->mapAnimationFriendlyNameTo_pScene.at(animationName).pAIScene->mAnimations[0];

	float TicksPerSecond = static_cast<float>(x->mTicksPerSecond != 0 ? x->mTicksPerSecond : 25.0 );

	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)x->mDuration);
	
	// use the "animation" file to look up these nodes
	// (need the matOffset information from the animation file)
	this->ReadNodeHeirarchy(AnimationTime, animationName, this->pScene->mRootNode, Identity);

	FinalTransformation.resize(this->mNumBones);
	Globals.resize(this->mNumBones);
	Offsets.resize(this->mNumBones);

	for ( unsigned int BoneIndex = 0; BoneIndex < this->mNumBones; BoneIndex++ )
	{
		FinalTransformation[BoneIndex] = this->mBoneInfo[BoneIndex].FinalTransformation;
		Globals[BoneIndex] = this->mBoneInfo[BoneIndex].ObjectBoneTransformation;
		Offsets[BoneIndex] = this->mBoneInfo[BoneIndex].BoneOffset;
	}
}

void cSimpleAssimpSkinnedMesh::LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &vertexBoneData)
{
	for ( unsigned int boneIndex = 0; boneIndex != Mesh->mNumBones; boneIndex++ )
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(Mesh->mBones[boneIndex]->mName.data);


		//	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> mMapping;
		// 	std::vector<sBoneInfo> mInfo;

		std::map<std::string, unsigned int>::iterator it = this->m_mapBoneNameToBoneIndex.find(BoneName);
		if ( it == this->m_mapBoneNameToBoneIndex.end() )
		{
			BoneIndex = this->mNumBones;
			this->mNumBones++;
			sBoneInfo bi;
			this->mBoneInfo.push_back(bi);

			this->mBoneInfo[BoneIndex].BoneOffset = AIMatrixToGLMMatrix(Mesh->mBones[boneIndex]->mOffsetMatrix);
			this->mBoneInfo[BoneIndex].boneName = BoneName;
			this->m_mapBoneNameToBoneIndex[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = it->second;
		}

		for ( unsigned int WeightIndex = 0; WeightIndex != Mesh->mBones[boneIndex]->mNumWeights; WeightIndex++ )
		{
			unsigned int VertexID = /*mMeshEntries[MeshIndex].BaseVertex +*/ Mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
			float Weight = Mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
			vertexBoneData[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
	return;
}

const aiNodeAnim* cSimpleAssimpSkinnedMesh::FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString boneName)
{
	for ( unsigned int ChannelIndex = 0; ChannelIndex != pAnimation->mNumChannels; ChannelIndex++ )
	{
		if ( pAnimation->mChannels[ChannelIndex]->mNodeName == boneName )
		{
			return pAnimation->mChannels[ChannelIndex];
		}
	}
	return 0;
}


unsigned int cSimpleAssimpSkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for ( unsigned int RotationKeyIndex = 0; RotationKeyIndex != pNodeAnim->mNumRotationKeys - 1; RotationKeyIndex++ )
	{
		if ( AnimationTime < (float)pNodeAnim->mRotationKeys[RotationKeyIndex+1].mTime )
		{
			return RotationKeyIndex;
		}
	}

	return 0;
}

unsigned int cSimpleAssimpSkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for ( unsigned int PositionKeyIndex = 0; PositionKeyIndex != pNodeAnim->mNumPositionKeys - 1; PositionKeyIndex++ )
	{
		if ( AnimationTime < (float)pNodeAnim->mPositionKeys[PositionKeyIndex+1].mTime )
		{
			return PositionKeyIndex;
		}
	}

	return 0;
}

unsigned int cSimpleAssimpSkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for ( unsigned int ScalingKeyIndex = 0; ScalingKeyIndex != pNodeAnim->mNumScalingKeys - 1; ScalingKeyIndex++ )
	{
		if ( AnimationTime < (float)pNodeAnim->mScalingKeys[ScalingKeyIndex+1].mTime )
		{
			return ScalingKeyIndex;
		}
	}

	return 0;
}

void cSimpleAssimpSkinnedMesh::ReadNodeHeirarchy(float AnimationTime, 
												 std::string animationName, 
												 const aiNode* pNode,
												 const glm::mat4 &ParentTransformMatrix)
{
	aiString NodeName(pNode->mName.data);

// Original version picked the "main scene" animation...
	const aiAnimation* pAnimation = this->pScene->mAnimations[0];

	// Search for the animation we want... 
	std::map< std::string /*animation FRIENDLY name*/,
		      sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(animationName);		// Animations

	// Did we find it? 
	if ( itAnimation != this->mapAnimationFriendlyNameTo_pScene.end() )
	{	
		// Yes, there is an animation called that...
		// ...replace the animation with the one we found
		pAnimation = reinterpret_cast<const aiAnimation*>( itAnimation->second.pAIScene->mAnimations[0] );
	}


	//aiMatrix4x4 NodeTransformation;

	// Transformation of the node in bind pose
	glm::mat4 NodeTransformation = AIMatrixToGLMMatrix( pNode->mTransformation );

	const aiNodeAnim* pNodeAnim = this->FindNodeAnimationChannel(pAnimation, NodeName);

	if ( pNodeAnim )
	{
		// Get interpolated scaling
		glm::vec3 scale;
		this->CalcGLMInterpolatedScaling(AnimationTime, pNodeAnim, scale);
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

		// Get interpolated rotation (quaternion)
		glm::quat ori;
		this->CalcGLMInterpolatedRotation(AnimationTime, pNodeAnim, ori);
		glm::mat4 RotationM = glm::mat4_cast(ori);
		
		// Get interpolated position 
		glm::vec3 pos;
		this->CalcGLMInterpolatedPosition(AnimationTime, pNodeAnim, pos);
		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), pos);
		
		
		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}
	//else
	//{
	//	// If there's no bone corresponding to this node, then it's something the animator used to 
	//	//	help make or position the model or animation. The point is that it DOESN'T change
	//	//	based on the animation frame, so take it just as is
	//	NodeTransformation = AIMatrixToGLMMatrix( pNode->mTransformation );
	//}

	glm::mat4 ObjectBoneTransformation = ParentTransformMatrix * NodeTransformation;

	std::map<std::string, unsigned int>::iterator it = this->m_mapBoneNameToBoneIndex.find(std::string(NodeName.data));
	if ( it != this->m_mapBoneNameToBoneIndex.end() )
	{
		unsigned int BoneIndex = it->second;
		this->mBoneInfo[BoneIndex].ObjectBoneTransformation = ObjectBoneTransformation;
		this->mBoneInfo[BoneIndex].FinalTransformation = this->mGlobalInverseTransformation 
		                                                 * ObjectBoneTransformation 
			                                             * this->mBoneInfo[BoneIndex].BoneOffset;
		//this->mBoneInfo[BoneIndex].FinalTransformation = GlobalTransformation 
		//	                                             * this->mBoneInfo[BoneIndex].BoneOffset;

	}
	else
	{
		int breakpoint = 0;
	}

	for ( unsigned int ChildIndex = 0; ChildIndex != pNode->mNumChildren; ChildIndex++ )
	{
		this->ReadNodeHeirarchy( AnimationTime, animationName,
								 pNode->mChildren[ChildIndex], ObjectBoneTransformation);
	}

}

void cSimpleAssimpSkinnedMesh::CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion &out)
{
	if ( pNodeAnim->mNumRotationKeys == 1 ) 
	{
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = this->FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();

	return;
}

void cSimpleAssimpSkinnedMesh::CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D &out)
{
	if ( pNodeAnim->mNumPositionKeys == 1 )
	{
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = this->FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	out = (EndPosition - StartPosition) * Factor + StartPosition;

	return;
}

void cSimpleAssimpSkinnedMesh::CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D &out)
{
	if ( pNodeAnim->mNumScalingKeys == 1 )
	{
		out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = this->FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	out = (EndScale - StartScale) * Factor + StartScale;

	return;
}

void cSimpleAssimpSkinnedMesh::CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat &out)
{
	if ( pNodeAnim->mNumRotationKeys == 1 ) 
	{
		out.w = pNodeAnim->mRotationKeys[0].mValue.w;
		out.x = pNodeAnim->mRotationKeys[0].mValue.x;
		out.y = pNodeAnim->mRotationKeys[0].mValue.y;
		out.z = pNodeAnim->mRotationKeys[0].mValue.z;
		return;
	}

	unsigned int RotationIndex = this->FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	
	glm::quat StartGLM = glm::quat(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
	glm::quat EndGLM = glm::quat(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

	out = glm::slerp(StartGLM, EndGLM, Factor);

	out = glm::normalize(out);

	return;
}

void cSimpleAssimpSkinnedMesh::CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3 &out)
{
	if ( pNodeAnim->mNumPositionKeys == 1 )
	{
		out.x = pNodeAnim->mPositionKeys[0].mValue.x;
		out.y = pNodeAnim->mPositionKeys[0].mValue.y;
		out.z = pNodeAnim->mPositionKeys[0].mValue.z;
		return;
	}

	unsigned int PositionIndex = this->FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	glm::vec3 start = glm::vec3(StartPosition.x, StartPosition.y, StartPosition.z);
	glm::vec3 end = glm::vec3(EndPosition.x, EndPosition.y, EndPosition.z);

	out = (end - start) * Factor + start;

	return;
}

void cSimpleAssimpSkinnedMesh::CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3 &out)
{
	if ( pNodeAnim->mNumScalingKeys == 1 )
	{
		out.x = pNodeAnim->mScalingKeys[0].mValue.x;
		out.y = pNodeAnim->mScalingKeys[0].mValue.y;
		out.z = pNodeAnim->mScalingKeys[0].mValue.z;
		return;
	}

	unsigned int ScalingIndex = this->FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	glm::vec3 start = glm::vec3(StartScale.x, StartScale.y, StartScale.z);
	glm::vec3 end = glm::vec3(EndScale.x, EndScale.y, EndScale.z);
	out = (end - start) * Factor + start;

	return;
}

float cSimpleAssimpSkinnedMesh::GetDuration(void)
{
	float duration = (float)(this->pScene->mAnimations[0]->mDuration / this->pScene->mAnimations[0]->mTicksPerSecond);

	return duration; 
}

// Original method (using cMesh, but we are using an sModelDrawInfo class)
//cMesh* cSimpleAssimpSkinnedMesh::CreateMeshObjectFromCurrentModel( unsigned int meshIndex /*=0*/)
//
// Returns NULL (0) if there is no mesh at that index
sModelDrawInfo* cSimpleAssimpSkinnedMesh::CreateModelDrawInfoObjectFromCurrentModel(unsigned int meshIndex/*=0*/)
{
	if ( this->pScene->mNumMeshes < meshIndex )
	{	// Doesn't have this mesh
		return NULL;
	}

	// Assume there is a valid mesh there
//	cMesh* pTheMesh = new cMesh();
	sModelDrawInfo* pTheMDI = new sModelDrawInfo();

	aiMesh* pAIMesh = this->pScene->mMeshes[meshIndex];

	pTheMDI->numberOfVertices = pAIMesh->mNumVertices;

	pTheMDI->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[ pTheMDI->numberOfVertices ];

	for ( int vertIndex = 0; vertIndex != pTheMDI->numberOfVertices; vertIndex++ )
	{
		sVertex_xyz_rgba_n_uv2_bt_4Bones* pCurVert = &(pTheMDI->pVertices[vertIndex] );

		aiVector3D* pAIVert =&(pAIMesh->mVertices[vertIndex]);

		pCurVert->x = pAIVert->x;
		pCurVert->y = pAIVert->y;
		pCurVert->z = pAIVert->z;

		// Colours
		if ( pAIMesh->GetNumColorChannels() > 0 )
		{
			pCurVert->r = this->pScene->mMeshes[0]->mColors[vertIndex]->r;
			pCurVert->g = this->pScene->mMeshes[0]->mColors[vertIndex]->g;
			pCurVert->b = this->pScene->mMeshes[0]->mColors[vertIndex]->b;
			pCurVert->a = this->pScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else
		{
			pCurVert->r = pCurVert->g = pCurVert->b = pCurVert->a = 1.0f;
		}

		// Normals
		if ( pAIMesh->HasNormals() )
		{
			pCurVert->nx = pAIMesh->mNormals[vertIndex].x;
			pCurVert->ny = pAIMesh->mNormals[vertIndex].y;
			pCurVert->nx = pAIMesh->mNormals[vertIndex].z;
		}

		// UVs
		if ( pAIMesh->GetNumUVChannels() > 0 )
		{	// Assume 1st channel is the 2D UV coordinates
			pCurVert->u0 = pAIMesh->mTextureCoords[0][vertIndex].x;
			pCurVert->v0 = pAIMesh->mTextureCoords[0][vertIndex].y;
			if ( pAIMesh->GetNumUVChannels() > 1 )
			{
				pCurVert->u1 = pAIMesh->mTextureCoords[1][vertIndex].x;
				pCurVert->v1 = pAIMesh->mTextureCoords[1][vertIndex].y;
			}
		}

		// Tangents and Bitangents (bi-normals)
		if ( pAIMesh->HasTangentsAndBitangents() )
		{
			pCurVert->tx = pAIMesh->mTangents[vertIndex].x;
			pCurVert->ty = pAIMesh->mTangents[vertIndex].y;
			pCurVert->tz = pAIMesh->mTangents[vertIndex].z;

			pCurVert->bx = pAIMesh->mBitangents[vertIndex].x;
			pCurVert->by = pAIMesh->mBitangents[vertIndex].y;
			pCurVert->bz = pAIMesh->mBitangents[vertIndex].z;
		}

		// Bone IDs are being passed OK
		pCurVert->boneID[0] = this->vecVertexBoneData[vertIndex].ids[0];
		pCurVert->boneID[1] = this->vecVertexBoneData[vertIndex].ids[1];
		pCurVert->boneID[2] = this->vecVertexBoneData[vertIndex].ids[2];
		pCurVert->boneID[3] = this->vecVertexBoneData[vertIndex].ids[3];

		// Weights are being passed OK
		pCurVert->boneWeights[0] = this->vecVertexBoneData[vertIndex].weights[0];
		pCurVert->boneWeights[1] = this->vecVertexBoneData[vertIndex].weights[1];
		pCurVert->boneWeights[2] = this->vecVertexBoneData[vertIndex].weights[2];
		pCurVert->boneWeights[3] = this->vecVertexBoneData[vertIndex].weights[3];


	}//for ( int vertIndex

	// Triangles
	pTheMDI->numberOfTriangles = pAIMesh->mNumFaces;

	// pTheMDI->numberOfIndices = pTheMDI->numberOfVertices * 3;
	pTheMDI->numberOfIndices = pTheMDI->numberOfTriangles*3;

	pTheMDI->pIndices = new unsigned int[pTheMDI->numberOfIndices];

	unsigned int index = 0;
	for ( unsigned int triIndex = 0; triIndex != pTheMDI->numberOfTriangles; 
		  triIndex++,	// next triangle is +1 
		  index += 3)	// next TRIANGLE index is +3 (3 indices per triangle)
	{
		aiFace* pAIFace = &(pAIMesh->mFaces[triIndex]);

		pTheMDI->pIndices[index + 0] = pAIFace->mIndices[0];	// Triangle index #0
		pTheMDI->pIndices[index + 1] = pAIFace->mIndices[1];	// Triangle index #1
		pTheMDI->pIndices[index + 2] = pAIFace->mIndices[2];	// Triangle index #2

	}//for ( unsigned int triIndex...

	pTheMDI->meshName = this->friendlyName;

	pTheMDI->CalcExtents();

	return pTheMDI;
}

