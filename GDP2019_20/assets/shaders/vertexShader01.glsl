#version 420

//uniform mat4 MVP;
uniform mat4 matModel;		// Model or World 
uniform mat4 matModelInverseTranspose;		// For normal calculation
uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform

//uniform sampler2D textSamp00;

layout (location = 0) in vec4 vColour;				// Was vec3
layout (location = 1) in vec4 vPosition;				// Was vec3
layout (location = 2) in vec4 vNormal;				// Vertex normal
layout (location = 3) in vec4 vUVx2;					// 2 x Texture coords
layout (location = 4) in vec4 vTangent;				// For bump mapping
layout (location = 5) in vec4 vBiNormal;				// For bump mapping
layout (location = 6) in vec4 vBoneID;				// For skinned mesh (FBX)
layout (location = 7) in vec4 vBoneWeight;			// For skinned mesh (FBX)
layout (location = 8) in mat4 instanceMatrix;

//out vec3 color;
//out vec4 vertWorld;			// Location of the vertex in the world
out vec4 fColour;	
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

uniform bool itsDeadJim;
uniform float offset;
uniform bool isInstanced;

const int MAXNUMBEROFBONES = 100;
uniform mat4 matBonesArray[MAXNUMBEROFBONES];
uniform bool isSkinnedMesh;

void main()
{
    vec4 vertPosition = vPosition;
	
    // mat4 matMVP = matProj * matView * matModel;

	if(itsDeadJim)
	{
		vertPosition.xyz *= (vNormal.xyz * offset);
	}

	if (isSkinnedMesh)
	{
		// *************************************************
		mat4 BoneTransform = matBonesArray[ int(vBoneID[0]) ] * vBoneWeight[0];
		     BoneTransform += matBonesArray[ int(vBoneID[1]) ] * vBoneWeight[1];
		     BoneTransform += matBonesArray[ int(vBoneID[2]) ] * vBoneWeight[2];
		     BoneTransform += matBonesArray[ int(vBoneID[3]) ] * vBoneWeight[3];

		// Apply the bone transform to the vertex:
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);

		vec4 vertAfterBoneTransform = BoneTransform * vertOriginal;
				
		mat4 matMVP = matProj * matView * matModel;		
		// Transform the updated vertex location (from the bone)
		//  and transform with model view projection matrix (as usual)
		gl_Position = matMVP * vertAfterBoneTransform;
		
		
		// Then we do the normals, etc.
		fVertWorldLocation = matModel * vertAfterBoneTransform;	
		
		// Updated "world" or "model" transform 
		mat4 matModelAndBone = matModel * BoneTransform;
		
		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = inverse(transpose(matModelAndBone)) * vec4(theNormal, 1.0f);
		fNormal.xyz = normalize(fNormal.xyz); 
		
		fColour = vColour;	
		fUVx2 = vUVx2;

		
		// *************************************************
	}
	else
	{	
		mat4 matModelCpy = matModel;
		mat4 matModelInverseTransposeCpy = matModelInverseTranspose;
		if(isInstanced)
		{
			matModelCpy = instanceMatrix;
			matModelInverseTransposeCpy = inverse(transpose(matModelCpy));
		}

		//vertPosition.xyz += ( vNormal.xyz * textOffset.x );		
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);
		
		mat4 matMVP = matProj * matView * matModelCpy;	

		gl_Position = matMVP * vertOriginal;
		
		// Vertex location in "world space"
		// Vec4 = mat4x4 * vec4
		fVertWorldLocation = matModelCpy * vertOriginal;
		
		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = matModelInverseTransposeCpy * vec4(theNormal, 1.0f);
		fNormal.xyz = normalize(fNormal.xyz); 
		
		// Pass the colour and UV unchanged.
		fColour = vColour;	
		fUVx2 = vUVx2;
	
	}
}
