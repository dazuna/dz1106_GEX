#include "cDebugRenderer.h"

#include "../GLCommon.h"

#include "COpenGLError.h"

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <sstream>		
#include <fstream>

#include "cGLRenderStateHelper.h"

#include <iostream>


// Used to hold the default sphere shape
extern float default_sphere_array [];
extern unsigned int default_sphere_array_size;


#include "cDebugRenderer_built_in_shaders.h"



// Used to compile the shader
void cDebugRenderer::m_parseStringIntoMultiLine( std::string singleLineSource, std::vector<std::string> &vecMultiline )
{
#ifdef _WIN32
	#pragma warning(disable : 4996)				// bitching about strcpy_s
#endif
//#ifdef _WIN64
//	#pragma warning(disable : 4996)				// bitching about strcpy_s
//#endif
	vecMultiline.clear();

	unsigned int stringsizeX4 = (unsigned int)singleLineSource.size() * 4;
	char* charSourceString = new char[stringsizeX4];
	strcpy(charSourceString, singleLineSource.c_str());

	char * pCharLine;
	pCharLine = strtok(charSourceString,"\n");
	//char *next_token = NULL;  
	//pCharLine = strtok_s(charSourceString,"\n",&next_token);

	while (pCharLine != NULL)
	{	// Push back this line
		vecMultiline.push_back( std::string(pCharLine) );
		pCharLine = strtok (NULL, "\n");
		//pCharLine = strtok_s(NULL,"\n",&next_token);
	}
	delete [] charSourceString;
	return;
//#undef _CRT_SECURE_NO_WARNINGS
}

bool cDebugRenderer::m_compileShaderFromSource( unsigned int &shaderID, std::vector<std::string> &vecMultilineSource, std::string &errorText )
{
	errorText = "";

	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(vecMultilineSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char*[numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset( arraySource, 0, numberOfLines );	

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{
		unsigned int numCharacters = (unsigned int)vecMultilineSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset( arraySource[indexLine], 0, numCharacters + 2 );

		// Copy line of source into array
		for ( unsigned int indexChar = 0; indexChar != vecMultilineSource[indexLine].length(); indexChar++ )
		{
			arraySource[indexLine][indexChar] = vecMultilineSource[indexLine][indexChar];
		}//for ( unsigned int indexChar = 0...
		
		// At a '\0' at end (just in case)
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

		// Or you could use the (unsecure) strcpy (string copy)
//		strcpy( arraySource[indexLine], shader.vecSource[indexLine].c_str() );

		// Or the "secure" version, where you specify the number of chars to copy
//		strcpy_s( arraySource[indexLine], 
//				  strlen( shader.vecSource[indexLine].c_str() + 1 ),	// Number of char in copy + 0 terminator
//				  shader.vecSource[indexLine].c_str() );

	}//for ( unsigned int indexLine = 0...

//******************************************************************************************
//	// Print it out (for debug)
//	std::cout << "Source (from char** array):" << std::endl;
//	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
//	{
//		std::cout << "(" << indexLine << "):\t" << arraySource[indexLine] << std::endl;
//	}
//	std::cout << "END" << std::endl;
//******************************************************************************************

	//const char* tempVertChar = "Hey there;";

	glShaderSource(shaderID, numberOfLines, arraySource, NULL);
    glCompileShader(shaderID); 

	// Get rid of the temp source "c" style array
	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{	// Delete this line
		delete [] arraySource[indexLine];
	}
	// And delete the original char** array
	delete [] arraySource;


	// Was there a compile error?	
//	std::string errorText = "";
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		delete [] pLogText;	// Oops

		return false;	// There WAS an error
	}

	return true;
}

bool cDebugRenderer::m_wasThereALinkError( unsigned int programID, std::string &error )
{
	error = "";	// No error

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if(wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		error.append(pLogText);

		delete [] pLogText;	

		// There WAS an error
		return true;	
	}
	
	// There WASN'T an error
	return false; 
}



bool cDebugRenderer::initialize(void)
{
	// Create our own, local, shader manager:
	// (this is deleted once it's used, but that's OK since we only 
	//  need it to get the shader 'name' or ID)

	std::vector<std::string> multiLineSource;
	std::string error;


//    ___     _     _     ___ _            _          
//   | _ \___(_)_ _| |_  / __| |_  __ _ __| |___ _ _  
//   |  _/ _ \ | ' \  _| \__ \ ' \/ _` / _` / -_) '_| 
//   |_| \___/_|_||_\__| |___/_||_\__,_\__,_\___|_|   
//                                                    
	//this->m_pShaderProg_Lines->vertShader.parseStringIntoMultiLine(this->m_vertexShaderSource);
	//this->m_pShaderProg_Lines->geomShader.parseStringIntoMultiLine(this->m_geometryShaderSource_Lines);
	//this->m_pShaderProg_Lines->fragShader.parseStringIntoMultiLine(this->m_fragmentShaderSource);
	
	GLuint pointVertexShaderID =	glCreateShader(GL_VERTEX_SHADER);
//	GLuint pointGeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint pointFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_vertexShaderSource_Points, multiLineSource );
	if ( ! this->m_compileShaderFromSource( pointVertexShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile point vertex shader: " + error;
		return false;
	}

	// There's no geometry shader for the point renderer

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_fragmentShaderSource_Points, multiLineSource );
	if ( ! this->m_compileShaderFromSource( pointFragmentShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile point fragment shader: " + error;
		return false;
	}
	
	this->m_PointShaderProgramID = glCreateProgram();
	glAttachShader( this->m_PointShaderProgramID, pointVertexShaderID);
	glAttachShader( this->m_PointShaderProgramID, pointFragmentShaderID);
	glLinkProgram( this->m_PointShaderProgramID );

	if ( this->m_wasThereALinkError( this->m_PointShaderProgramID, error ) )
	{
		this->m_lastError = "Can't link point shader: " + error;
		return false;
	}


	// The shader was compiled, so get the shader program number
//	this->m_pShaderProg_Lines->shaderProgramID = shaderManager.getIDFromFriendlyName("debugShaderLines");

	// Get the uniform variable locations 
	glUseProgram(this->m_PointShaderProgramID);
	this->m_PointShader_matModelUniformLoc = glGetUniformLocation(this->m_PointShaderProgramID, "mModel");
	this->m_PointShader_matViewUniformLoc = glGetUniformLocation(this->m_PointShaderProgramID, "mView");
	this->m_PointShader_matProjectionUniformLoc = glGetUniformLocation(this->m_PointShaderProgramID, "mProjection");

	//std::cout << "Lines::mModel = " << this->m_pShaderProg_Lines->matModelUniformLoc << std::endl;
	//std::cout << "Lines::mView = " << this->m_pShaderProg_Lines->matViewUniformLoc << std::endl;
	//std::cout << "Lines::mProjection = " << this->m_pShaderProg_Lines->matProjectionUniformLoc << std::endl;

	glUseProgram(0);


//    _    _            ___ _            _         
//   | |  (_)_ _  ___  / __| |_  __ _ __| |___ _ _ 
//   | |__| | ' \/ -_) \__ \ ' \/ _` / _` / -_) '_|
//   |____|_|_||_\___| |___/_||_\__,_\__,_\___|_|  
//                                                 
	//this->m_pShaderProg_Lines->vertShader.parseStringIntoMultiLine(this->m_vertexShaderSource);
	//this->m_pShaderProg_Lines->geomShader.parseStringIntoMultiLine(this->m_geometryShaderSource_Lines);
	//this->m_pShaderProg_Lines->fragShader.parseStringIntoMultiLine(this->m_fragmentShaderSource);
	
	GLuint lineVertexShaderID =	glCreateShader(GL_VERTEX_SHADER);
	GLuint lineGeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint lineFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_vertexShaderSource_Lines, multiLineSource );
	if ( ! this->m_compileShaderFromSource( lineVertexShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line vertex shader: " + error;
		return false;
	}

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_geometryShaderSource_Lines, multiLineSource );
	if ( ! this->m_compileShaderFromSource( lineGeometryShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line geometry shader: " + error;
		return false;
	}

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_fragmentShaderSource_Lines, multiLineSource );
	if ( ! this->m_compileShaderFromSource( lineFragmentShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line fragment shader: " + error;
		return false;
	}
	
	this->m_LineShaderProgramID = glCreateProgram();
	glAttachShader( this->m_LineShaderProgramID, lineVertexShaderID );
	glAttachShader( this->m_LineShaderProgramID, lineGeometryShaderID );
	glAttachShader( this->m_LineShaderProgramID, lineFragmentShaderID );
	glLinkProgram( this->m_LineShaderProgramID );

	if ( this->m_wasThereALinkError( this->m_LineShaderProgramID, error ) )
	{
		this->m_lastError = "Can't link line shader: " + error;
		return false;
	}


	// The shader was compiled, so get the shader program number
//	this->m_pShaderProg_Lines->shaderProgramID = shaderManager.getIDFromFriendlyName("debugShaderLines");

	// Get the uniform variable locations 
	glUseProgram(this->m_LineShaderProgramID);
	this->m_LineShader_matModelUniformLoc = glGetUniformLocation(this->m_LineShaderProgramID, "mModel");
	this->m_LineShader_matViewUniformLoc = glGetUniformLocation(this->m_LineShaderProgramID, "mView");
	this->m_LineShader_matProjectionUniformLoc = glGetUniformLocation(this->m_LineShaderProgramID, "mProjection");

	//std::cout << "Lines::mModel = " << this->m_pShaderProg_Lines->matModelUniformLoc << std::endl;
	//std::cout << "Lines::mView = " << this->m_pShaderProg_Lines->matViewUniformLoc << std::endl;
	//std::cout << "Lines::mProjection = " << this->m_pShaderProg_Lines->matProjectionUniformLoc << std::endl;

	glUseProgram(0);

//    _____    _                _       ___ _            _         
//   |_   _| _(_)__ _ _ _  __ _| |___  / __| |_  __ _ __| |___ _ _ 
//     | || '_| / _` | ' \/ _` | / -_) \__ \ ' \/ _` / _` / -_) '_|
//     |_||_| |_\__,_|_||_\__, |_\___| |___/_||_\__,_\__,_\___|_|  
//                        |___/                                    

	GLuint triangleVertexShaderID =		glCreateShader(GL_VERTEX_SHADER);
	GLuint triangleGeometryShaderID =	glCreateShader(GL_GEOMETRY_SHADER);
	GLuint triangleFragmentShaderID =	glCreateShader(GL_FRAGMENT_SHADER);

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_vertexShaderSource_Triangles, multiLineSource );
	if ( ! this->m_compileShaderFromSource( triangleVertexShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line vertex shader: " + error;
		return false;
	}

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_geometryShaderSource_Triangles, multiLineSource );
	if ( ! this->m_compileShaderFromSource( triangleGeometryShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line geometry shader: " + error;
		return false;
	}

	multiLineSource.clear();
	this->m_parseStringIntoMultiLine( this->m_fragmentShaderSource_Triangles, multiLineSource );
	if ( ! this->m_compileShaderFromSource( triangleFragmentShaderID, multiLineSource, error ) )
	{
		this->m_lastError = "Can't compile line fragment shader: " + error;
		return false;
	}
	
	this->m_TriangleShaderProgramID = glCreateProgram();
	glAttachShader( this->m_TriangleShaderProgramID, triangleVertexShaderID );
	glAttachShader( this->m_TriangleShaderProgramID, triangleGeometryShaderID );
	glAttachShader( this->m_TriangleShaderProgramID, triangleFragmentShaderID );
	glLinkProgram( this->m_TriangleShaderProgramID );

	if ( this->m_wasThereALinkError( this->m_TriangleShaderProgramID, error ) )
	{
		this->m_lastError = "Can't link line shader: " + error;
		return false;
	}


	// Get the uniform variable locations 
	glUseProgram(this->m_TriangleShaderProgramID);
	this->m_TriangleShader_matModelUniformLoc = glGetUniformLocation(this->m_TriangleShaderProgramID, "mModel");
	this->m_TriangleShader_matViewUniformLoc = glGetUniformLocation(this->m_TriangleShaderProgramID, "mView");
	this->m_TriangleShader_matProjectionUniformLoc = glGetUniformLocation(this->m_TriangleShaderProgramID, "mProjection");

	std::cout << "Triangles::mModel = " << this->m_TriangleShader_matModelUniformLoc << std::endl;
	std::cout << "Triangles::mView = " << this->m_TriangleShader_matViewUniformLoc << std::endl;
	std::cout << "Triangles::mProjection = " << this->m_TriangleShader_matProjectionUniformLoc << std::endl;

	glUseProgram(0);


	// Set up the VBOs...
	if ( ! this->resizeBufferForPoints(cDebugRenderer::DEFAULTNUMBEROFPOINTS) )
	{
		return false;
	}
	if ( ! this->resizeBufferForLines(cDebugRenderer::DEFAULTNUMBEROFLINES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForTriangles(cDebugRenderer::DEFAULTNUMBEROFTRIANGLES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForMeshes(cDebugRenderer::DEFAULT_MESHBUFFERSIZE_PADDING_RATIO) )
	{
		return false;
	}
	
	// Load the default sphere
//	this->QnD_loadHeaderArrayInto_vecTri( default_sphere_array, default_sphere_array_size, vecSphereTris);
//	this->generateIsoSphere( 1, 1.0f,  vecSphereTris );
//	this->loadDebugMesh( iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME, vecSphereTris );
//	this->loadDebugMesh("sphere", vecSphereTris);

	std::vector<iDebugRenderer::sDebugTri> vecSphereTris;
	this->generateDefaultSphereMesh(vecSphereTris);
	this->loadDebugMesh(iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME, vecSphereTris);


	return true;
}

bool cDebugRenderer::resizeBufferForPoints(unsigned int newNumberOfPoints)
{
	// Erase any exisiting buffers 
	if (this->m_VAOBufferInfoPoints.bIsValid)
	{	// Assume it exists, so delete it
		delete[] this->m_VAOBufferInfoPoints.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoPoints.vertex_buffer_ID));

		glDeleteVertexArrays(1, &(this->m_VAOBufferInfoPoints.VAO_ID));
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfPoints = (unsigned int)(newNumberOfPoints * 1.1);

	this->m_VAOBufferInfoPoints.bufferSizeObjects = newNumberOfPoints;
	this->m_VAOBufferInfoPoints.bufferSizeVertices = newNumberOfPoints * 1;
	this->m_VAOBufferInfoPoints.bufferSizeBytes = 0;
	this->m_VAOBufferInfoPoints.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoPoints.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoPoints.shaderID = this->m_PointShaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoPoints);

	return true;


}

//static
const float cDebugRenderer::DEFAULT_MESHBUFFERSIZE_PADDING_RATIO = 1.10f; //or 10 %

bool cDebugRenderer::resizeBufferForMeshes(float meshBufferPaddingRatio /*=DEFAULT_MESHBUFFERSIZE_PADDING_RATIO*/)
{
	//TODO


	return true;
}


bool cDebugRenderer::resizeBufferForLines(unsigned int newNumberOfLines)
{
	// Erase any exisiting buffers 
	if ( this->m_VAOBufferInfoLines.bIsValid )
	{	// Assume it exists, so delete it
		delete [] this->m_VAOBufferInfoLines.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoLines.vertex_buffer_ID) );

		glDeleteVertexArrays( 1, &(this->m_VAOBufferInfoLines.VAO_ID) );
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfLines = (unsigned int)(newNumberOfLines * 1.1);	

	this->m_VAOBufferInfoLines.bufferSizeObjects = newNumberOfLines;
	this->m_VAOBufferInfoLines.bufferSizeVertices = newNumberOfLines * 2;
	this->m_VAOBufferInfoLines.bufferSizeBytes = 0;
	this->m_VAOBufferInfoLines.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoLines.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoLines.shaderID = this->m_LineShaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoLines);

	return true;
}

bool cDebugRenderer::resizeBufferForTriangles(unsigned int newNumberOfTriangles)
{
	// Erase any exisiting buffers 
	if ( this->m_VAOBufferInfoTriangles.bIsValid )
	{	// Assume it exists, so delete it
		delete [] this->m_VAOBufferInfoTriangles.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoTriangles.vertex_buffer_ID) );

		glDeleteVertexArrays( 1, &(this->m_VAOBufferInfoTriangles.VAO_ID) );
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfTriangles = (unsigned int)(newNumberOfTriangles * 1.1);	

	this->m_VAOBufferInfoTriangles.bufferSizeObjects = newNumberOfTriangles;
	this->m_VAOBufferInfoTriangles.bufferSizeVertices = newNumberOfTriangles * 3;
	this->m_VAOBufferInfoTriangles.bufferSizeBytes = 0;
	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoTriangles.shaderID = this->m_TriangleShaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoTriangles);
}

bool cDebugRenderer::m_InitBuffer(sVAOInfoDebug &VAOInfo)
{
//	glUseProgram(this->m_pShaderProg->shaderProgramID);
	glUseProgram(VAOInfo.shaderID);

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &(VAOInfo.VAO_ID) );
	glBindVertexArray( VAOInfo.VAO_ID );


    glGenBuffers(1, &(VAOInfo.vertex_buffer_ID) );
    glBindBuffer(GL_ARRAY_BUFFER, VAOInfo.vertex_buffer_ID);

	VAOInfo.pLocalVertexArray = new sVertex_xyzw_rgba[VAOInfo.bufferSizeVertices];
	
	// Clear buffer
	VAOInfo.bufferSizeBytes = sizeof(sVertex_xyzw_rgba) * VAOInfo.bufferSizeVertices;
	memset(VAOInfo.pLocalVertexArray, 0, VAOInfo.bufferSizeBytes);

	// Copy the local vertex array into the GPUs memory
    glBufferData(GL_ARRAY_BUFFER, 
				 VAOInfo.bufferSizeBytes,		
				 VAOInfo.pLocalVertexArray,
				 GL_DYNAMIC_DRAW);

	// **DON'T** Get rid of local vertex array!
	// (We will need to copy the debug objects into this later!!)
	//delete [] pVertices;

	// Now set up the vertex layout (for this shader):
	//
    //	in vec4 vPosition;                           
    //	in vec4 vColour;                               
	//
	GLuint vpos_location = glGetAttribLocation(VAOInfo.shaderID, "vPosition");		// program, "vPos");	// 6
    GLuint vcol_location = glGetAttribLocation(VAOInfo.shaderID, "vColour");		// program, "vCol");	// 13

	// Size of the vertex we are using in the array.
	// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof(sVertex_xyzw_rgba);

    glEnableVertexAttribArray(vpos_location);
	const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, x );
    glVertexAttribPointer(vpos_location, 
						  4,					//	in vec4 vPosition; 	
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)) );	// 64-bit


    glEnableVertexAttribArray(vcol_location);
	const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, r );
    glVertexAttribPointer(vcol_location, 
						  4,					//	in vec4 vColour; 
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,		
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R_IN_CVERTEX)) );	// 64-bit
	// *******************************************************************

	VAOInfo.bIsValid = true;

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);

	glUseProgram(0);

	return true;
}

bool cDebugRenderer::IsOK(void)
{
	//TODO
	return true;
}


cDebugRenderer::cDebugRenderer()
{
	this->m_vertexShaderSource_Points = cDebugRenderer::DEFAULT_VERT_SHADER_SOURCE_POINTS;
	this->m_fragmentShaderSource_Points = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_POINTS;

	this->m_vertexShaderSource_Lines = cDebugRenderer::DEFAULT_VERT_SHADER_SOURCE_LINES;
	this->m_geometryShaderSource_Lines = cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_LINES;
	this->m_fragmentShaderSource_Lines = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_LINES;

	this->m_vertexShaderSource_Triangles = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_TRIANGLES;
	this->m_geometryShaderSource_Triangles = cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_TRIANGLES;
	this->m_fragmentShaderSource_Triangles = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_TRIANGLES;

	//this->m_pShaderProg_Lines = new cShaderProgramInfo();
	//this->m_pShaderProg_Triangles = new cShaderProgramInfo();

	this->m_LineShaderProgramID = 0;
	this->m_TriangleShaderProgramID = 0;

	this->m_pGLRenderState = new cGLRenderStateHelper();

	this->m_PointSize = 1.0f; 

	return;
}

cDebugRenderer::~cDebugRenderer()
{
	if ( this->m_pGLRenderState ) { delete this->m_pGLRenderState; }

	//if (this->m_pShaderProg_Lines)
	//{
	//	delete this->m_pShaderProg_Lines;
	//}

	//if (this->m_pShaderProg_Triangles)
	//{
	//	delete this->m_pShaderProg_Triangles;
	//}
	return;
}

void cDebugRenderer::setPointSize(float newSize)
{
	if (newSize < 1.0f)
	{
		return;
	}
	this->m_PointSize = newSize;
	return;
}


void cDebugRenderer::RenderDebugObjects(const glm::mat4 &matCameraView, const glm::mat4 &matProjection, double deltaTime)
{
	glm::mat4 matView = matCameraView;
	glm::mat4 matProj = matProjection;

	this->m_copyPointsIntoRenderBuffer(deltaTime);
	this->m_RenderDebugPoints(matView, matProj, deltaTime);

	this->m_copyLinesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugLines(matView, matProj, deltaTime);

	this->m_copyTrianglesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugTriangles(matView, matProj, deltaTime);

	return;
}

cDebugRenderer::sGLDrawState::sGLDrawState()
{
	// Save existing params
	this->GL_depth_test_state = GL_TRUE;
	// This has two values, GL_FRONT and GL_BACK
	this->GL_polygon_mode_state[0] = GL_FILL; /*GL_FRONT*/
	this->GL_polygon_mode_state[1] = GL_FILL; /*GL_BACK*/ 				// 6914
	this->GL_cull_face_enabled_state = GL_TRUE;
	this->GL_cull_face_mode_state = GL_BACK;
	this->currentProgram = 0;
	this->pointSize = 1.0f;
	this->pointSizeEnabled = GL_FALSE;
};

void cDebugRenderer::m_SaveGLState( cDebugRenderer::sGLDrawState &curGLState )
{
	glGetBooleanv( GL_DEPTH_TEST, &(curGLState.GL_depth_test_state) );				// glEnable(GL_DEPTH_TEST);
	// This returns two values, GL_FRONT and GL_BACK
	glGetIntegerv( GL_POLYGON_MODE, curGLState.GL_polygon_mode_state );			// glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glGetBooleanv( GL_CULL_FACE, &(curGLState.GL_cull_face_enabled_state) );			// glEnable(GL_CULL_FACE);
	glGetIntegerv( GL_CULL_FACE_MODE, &(curGLState.GL_cull_face_mode_state) );			// glCullFace(GL_BACK);

	glGetIntegerv(GL_CURRENT_PROGRAM, &(curGLState.currentProgram) );


	glGetBooleanv(GL_PROGRAM_POINT_SIZE, &(curGLState.pointSizeEnabled) );
	glGetFloatv(GL_POINT_SIZE, &(curGLState.pointSize) );

	return;
}

void cDebugRenderer::m_RestoreGLState( const cDebugRenderer::sGLDrawState &curGLState )
{
	glPolygonMode( GL_FRONT, curGLState.GL_polygon_mode_state[0] );	
	glPolygonMode( GL_BACK, curGLState.GL_polygon_mode_state[1] );		

	if ( curGLState.GL_cull_face_enabled_state == GL_TRUE )
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	glCullFace(curGLState.GL_cull_face_mode_state);

	if ( curGLState.GL_depth_test_state == GL_TRUE )
	{
		glEnable(GL_DEPTH_TEST);		
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	// Restore the old shader program
	glUseProgram( curGLState.currentProgram );

	glPointSize( curGLState.pointSize );

	if ( curGLState.pointSizeEnabled == GL_TRUE )
	{
		glEnable(GL_PROGRAM_POINT_SIZE);
	}
	else
	{
		glDisable(GL_PROGRAM_POINT_SIZE);
	}

	return;
}



void cDebugRenderer::m_RenderDebugTriangles(const glm::mat4 &matCameraView, const glm::mat4 &matProjection, double deltaTime)
{
	// Get current GL state:
	sGLDrawState curGLState;
	this->m_SaveGLState( curGLState );

	// Start rendering 
	glUseProgram(this->m_TriangleShaderProgramID);

	glUniformMatrix4fv( this->m_TriangleShader_matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_TriangleShader_matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_TriangleShader_matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(50.0f);

	// Draw triangles
	glBindVertexArray( this->m_VAOBufferInfoTriangles.VAO_ID );
	glDrawArrays( GL_POINTS, 
	//glDrawArrays( GL_TRIANGLES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw );
	glBindVertexArray( 0 );

	glUseProgram(0);

	// Put everything back as it was 
	this->m_RestoreGLState( curGLState );


	return;
}

void cDebugRenderer::m_RenderDebugLines(const glm::mat4 &matCameraView, const glm::mat4 &matProjection, double deltaTime)
{
	// Get current GL state:
	sGLDrawState curGLState;
	this->m_SaveGLState( curGLState );

	// Start rendering 
	glUseProgram(this->m_LineShaderProgramID);

	glUniformMatrix4fv( this->m_LineShader_matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_LineShader_matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_LineShader_matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(50.0f);


	// Draw Lines
	glBindVertexArray( this->m_VAOBufferInfoLines.VAO_ID );
	glDrawArrays( GL_LINES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoLines.numberOfVerticesToDraw );

	glBindVertexArray( 0 );

	glUseProgram(0);

	// Put everything back as it was 
	this->m_RestoreGLState( curGLState );

	return;
}

void cDebugRenderer::m_RenderDebugPoints(const glm::mat4 &matCameraView, const glm::mat4 &matProjection, double deltaTime)
{
	// Get current GL state:
	sGLDrawState curGLState;
	this->m_SaveGLState( curGLState );

	// Start rendering 
	glUseProgram(this->m_PointShaderProgramID);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(this->m_PointSize);
//	glPointSize(10.0f);

	glUniformMatrix4fv(this->m_PointShader_matViewUniformLoc, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(matCameraView));
	glUniformMatrix4fv(this->m_PointShader_matProjectionUniformLoc, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(matProjection));
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv(this->m_PointShader_matModelUniformLoc, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(glm::mat4(1.0f)));


	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(50.0f);

	// Draw Lines
	glBindVertexArray(this->m_VAOBufferInfoPoints.VAO_ID);
	glDrawArrays(GL_POINTS,
//	glDrawArrays(GL_LINES,
				 0,		// 1st vertex
				 this->m_VAOBufferInfoPoints.numberOfVerticesToDraw);

	glBindVertexArray(0);

	glUseProgram(0);
	// Put everything back as it was 
	this->m_RestoreGLState( curGLState );

	return;
}


void cDebugRenderer::m_copyTrianglesIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drTri> vecTriTemp;

	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = (unsigned int)this->m_vecTriangles.size();

	// Is the draw buffer big enough? 
	if ( this->m_VAOBufferInfoTriangles.bufferSizeObjects < this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw )
	{
		// Resize the buffer
		this->resizeBufferForTriangles( this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw );
	}

	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw
		= this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw * 3;	// Triangles

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int triIndex = 0;		// index of the triangle buffer
	for (; triIndex != this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw; 
		   triIndex++, vertexIndex += 3)
	{
		drTri& curTri = this->m_vecTriangles[triIndex];
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].x = curTri.v[0].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].y = curTri.v[0].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].z = curTri.v[0].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].x = curTri.v[1].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].y = curTri.v[1].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].z = curTri.v[1].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].x = curTri.v[2].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].y = curTri.v[2].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].z = curTri.v[2].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].a = 1.0f;

		curTri.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curTri.lifeTime > 0.0f)
		{
			vecTriTemp.push_back(curTri);
		}
	}//for (; 


	// Clear the triangle list and push back the persistent ones
	this->m_vecTriangles.clear();
	for (std::vector<drTri>::iterator itTri = vecTriTemp.begin(); itTri != vecTriTemp.end(); itTri++)
	{
		this->m_vecTriangles.push_back(*itTri);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoTriangles.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
		numberOfBytesToCopy,
		this->m_VAOBufferInfoTriangles.pLocalVertexArray,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
//		error = decodeGLErrorFromEnum(err, errDetails);
		error = COpenGLError::TranslateErrorEnum(err);
	}

	//	numberOfBytesToCopy,
	//	this->m_VAOBufferInfoTriangles.pLocalVertexArray,
	//	GL_DYNAMIC_DRAW);
	
	//	void* pGPUBuff = glMapBuffer( this->m_VAOBufferInfoTriangles.vertex_buffer_ID,
     //                             GL_COPY_WRITE_BUFFER);
	//memcpy(
	//	this->m_VAOBufferInfoTriangles.pLocalVertexArray,
	//	pGPUBuff,
	//	numberOfBytesToCopy);

//	glUnmapBuffer(this->m_VAOBufferInfoTriangles.vertex_buffer_ID);

	return;
}


void cDebugRenderer::m_copyLinesIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drLine> vecLinesTemp;

	this->m_VAOBufferInfoLines.numberOfObjectsToDraw = (unsigned int)this->m_vecLines.size();

	// Is the draw buffer big enough? 
	if ( this->m_VAOBufferInfoLines.bufferSizeObjects < this->m_VAOBufferInfoLines.numberOfObjectsToDraw )
	{
		// Resize the buffer
		this->resizeBufferForLines( this->m_VAOBufferInfoLines.numberOfObjectsToDraw );
	}

	this->m_VAOBufferInfoLines.numberOfVerticesToDraw
		= this->m_VAOBufferInfoLines.numberOfObjectsToDraw * 2;	// Lines

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int lineIndex = 0;		// index of the triangle buffer
	for (; lineIndex != this->m_VAOBufferInfoLines.numberOfObjectsToDraw; 
		   lineIndex++, vertexIndex += 2)
	{
		drLine& curLine = this->m_vecLines[lineIndex];
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].x = curLine.points[0].x;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].y = curLine.points[0].y;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].z = curLine.points[0].z;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].w = 1.0f;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].r = curLine.colour.r;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].g = curLine.colour.g;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].b = curLine.colour.b;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].a = 1.0f;

		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].x = curLine.points[1].x;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].y = curLine.points[1].y;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].z = curLine.points[1].z;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].w = 1.0f;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].r = curLine.colour.r;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].g = curLine.colour.g;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].b = curLine.colour.b;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].a = 1.0f;

		curLine.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curLine.lifeTime > 0.0f)
		{
			vecLinesTemp.push_back(curLine);
		}
	}//for (; 


	// Clear the triangle list and push back the persistent ones
	this->m_vecLines.clear();
	for (std::vector<drLine>::iterator itLine = vecLinesTemp.begin(); itLine != vecLinesTemp.end(); itLine++)
	{
		this->m_vecLines.push_back(*itLine);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoLines.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoLines.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
		numberOfBytesToCopy,
		this->m_VAOBufferInfoLines.pLocalVertexArray,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
//		error = decodeGLErrorFromEnum(err, errDetails);
		error = COpenGLError::TranslateErrorEnum(err);
	}
	return;
}

void cDebugRenderer::m_copyPointsIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drPoint> vecPointsTemp;

	this->m_VAOBufferInfoPoints.numberOfObjectsToDraw = (unsigned int)this->m_vecPoints.size();

	// Is the draw buffer big enough? 
	if (this->m_VAOBufferInfoPoints.bufferSizeObjects < this->m_VAOBufferInfoPoints.numberOfObjectsToDraw)
	{
		// Resize the buffer
		this->resizeBufferForPoints(this->m_VAOBufferInfoPoints.numberOfObjectsToDraw);
	}

	this->m_VAOBufferInfoPoints.numberOfVerticesToDraw
		= this->m_VAOBufferInfoPoints.numberOfObjectsToDraw * 1;	// Points

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int pointIndex = 0;		// index of the triangle buffer
	for (; pointIndex != this->m_VAOBufferInfoPoints.numberOfObjectsToDraw;
		 pointIndex++, vertexIndex += 1)
	{
		drPoint& curPoint = this->m_vecPoints[pointIndex];
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].x = curPoint.xyz.x;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].y = curPoint.xyz.y;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].z = curPoint.xyz.z;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].w = curPoint.pointSize;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].r = curPoint.colour.r;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].g = curPoint.colour.g;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].b = curPoint.colour.b;
		this->m_VAOBufferInfoPoints.pLocalVertexArray[vertexIndex + 0].a = 1.0f;


		curPoint.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curPoint.lifeTime > 0.0f)
		{
			vecPointsTemp.push_back(curPoint);
		}
	}//for (; 


	// Clear the point list and push back the persistent ones
	this->m_vecPoints.clear();
	for (std::vector<drPoint>::iterator itPoint = vecPointsTemp.begin(); itPoint != vecPointsTemp.end(); itPoint++)
	{
		this->m_vecPoints.push_back(*itPoint);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoPoints.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoPoints.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
				 numberOfBytesToCopy,
				 this->m_VAOBufferInfoPoints.pLocalVertexArray,
				 GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
		//		error = decodeGLErrorFromEnum(err, errDetails);
		error = COpenGLError::TranslateErrorEnum(err);
	}

	return;
}

void cDebugRenderer::addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drTri tempTri(v1XYZ, v2XYZ, v3XYZ, colour, lifeTime);
	this->addTriangle(tempTri);
	return;
}


void cDebugRenderer::addTriangle(drTri &tri)
{
	this->m_vecTriangles.push_back(tri);
	return;
}

void cDebugRenderer::addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drLine tempLine(startXYZ, endXYZ, colour, lifeTime);
	this->addLine(tempLine);
	return;
}

void cDebugRenderer::addLine(drLine &line)
{
	this->m_vecLines.push_back(line);
	return;
}


void cDebugRenderer::addPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drPoint tempPoint(xyz, colour, lifeTime, 1.0f);
	this->addPoint(tempPoint);
	return;
}

void cDebugRenderer::addPointPointSize(glm::vec3 xyz, glm::vec3 colour, float pointSize, float lifeTime/*=0.0f*/ )
{
	drPoint tempPoint(xyz, colour, lifeTime, pointSize);
	this->addPoint(tempPoint);
	return;
}

void cDebugRenderer::addPoint(drPoint &point)
{
	this->m_vecPoints.push_back(point);
	return;
}

// Replaces the DrawDebugSphere
void cDebugRenderer::addDebugSphere(glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	iDebugRenderer::sDebugMesh sphereMesh;
	sphereMesh.name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;
	sphereMesh.scale = scale;
	sphereMesh.xyz = xyz;
	sphereMesh.colour = colour;
	sphereMesh.lifeTime = lifeTime;
	this->m_vecMeshes.push_back(sphereMesh);
	return;
}

void cDebugRenderer::addDebugMesh(std::string meshName, glm::vec3 xyz, glm::vec3 orientXYZ_Euler, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	iDebugRenderer::sDebugMesh debugMesh;
	debugMesh.name = meshName;
	debugMesh.scale = scale;
	debugMesh.xyz = xyz;
	debugMesh.qOrientation = glm::quat(orientXYZ_Euler);
	debugMesh.colour = colour;
	debugMesh.lifeTime = lifeTime;
	this->m_vecMeshes.push_back(debugMesh);
	return;
}

void cDebugRenderer::addDebugMesh(std::string meshName, glm::vec3 xyz, glm::quat orientXYZ_Quaternion, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	iDebugRenderer::sDebugMesh debugMesh;
	debugMesh.name = meshName;
	debugMesh.scale = scale;
	debugMesh.xyz = xyz;
	debugMesh.qOrientation = orientXYZ_Quaternion;
	debugMesh.colour = colour;
	debugMesh.lifeTime = lifeTime;
	this->m_vecMeshes.push_back(debugMesh);
	return;
}


std::string cDebugRenderer::getLastError(bool bClearError /*=true*/)
{
	std::string theLastError = this->m_lastError;
	if ( bClearError )
	{
		this->m_lastError = "";
	}
	return theLastError;
}



// *********************************************************
//	 ___      _                ___ _                      
//	|   \ ___| |__ _  _ __ _  / __| |_  __ _ _ __  ___ ___
//	| |) / -_) '_ \ || / _` | \__ \ ' \/ _` | '_ \/ -_|_-<
//	|___/\___|_.__/\_,_\__, | |___/_||_\__,_| .__/\___/__/
//	                   |___/                |_|           
//

//static 
const std::string iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME = "DSPHERE";

iDebugRenderer::sDebugTri::sDebugTri()
{
	this->v[0] = glm::vec3(0.0f); this->v[1] = glm::vec3(0.0f); this->v[2] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 1.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v[3], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine()
{
	this->points[0] = glm::vec3(0.0f);
	this->points[0] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);		// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}
iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 points[2], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

/*static*/ 
const float cDebugRendererDEFAULT_POINT_SIZE = 1.0f;


iDebugRenderer::sDebugPoint::sDebugPoint()
{
	this->xyz = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->pointSize = cDebugRendererDEFAULT_POINT_SIZE;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz)		// colour=1,1,1		lifeTime=0.0f	pointSize=1.0f
{
	this->xyz = xyz;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->pointSize = 1.0f;
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}
iDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/, float pointSize/*=1.0f*/)
{
	this->xyz = xyz;
	this->colour = colour;
	this->pointSize = pointSize;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh()
{
	this->name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;	// = "DSPHERE";
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

// Assumes a size of 1, colour white
iDebugRenderer::sDebugMesh::sDebugMesh(std::string name)
{
	this->name = name;
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	return;
	this->bIgnorDepthBufferOnDraw = true;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 EulerOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(EulerOrientation);
	this->scale = scale;
	this->colour = colour;
	return;
	this->bIgnorDepthBufferOnDraw = true;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::quat qOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = qOrientation;
	this->scale = scale;
	this->colour = colour;
	return;
	this->bIgnorDepthBufferOnDraw = true;
}
//
// *********************************************************


