#version 420

//uniform mat4 MVP;
uniform mat4 matModel;		// Model or World 
uniform mat4 matModelInverseTranspose;		// For normal calculation
uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform

//uniform sampler2D textSamp00;

in vec4 vColour;				// Was vec3
in vec4 vPosition;				// Was vec3
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords

//out vec3 color;
//out vec4 vertWorld;			// Location of the vertex in the world
out vec4 fColour;	
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

uniform bool itsDeadJim;
uniform float offset;

void main()
{
    vec4 vertPosition = vPosition;
	
    mat4 matMVP = matProj * matView * matModel;

	if(itsDeadJim)
	{
		vertPosition.xyz *= (vNormal.xyz * offset);
	}
	
	gl_Position = matMVP * vec4(vertPosition.xyz, 1.0);
	
	// Vertex location in "world space"
	// Vec4 = mat4x4 * vec4
	fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);		
	
	//mat4 matModelInverseTranspose = inverse(transpose(matModel));
	
	vec3 theNormal = normalize(vNormal.xyz);
 	fNormal = matModelInverseTranspose * vec4(theNormal, 1.0f);
	fNormal.xyz = normalize(fNormal.xyz); 
	
	// Pass the colour and UV unchanged.
    fColour = vColour;	
	fUVx2 = vUVx2;
	
	// 
//	vec3 tex0_RGB = texture( textSamp00, fUVx2.st ).rgb;
//	if ( tex0_RGB.r <= 0.01f )		// Basically "black"
//	{
//		discard;
//	}
}
