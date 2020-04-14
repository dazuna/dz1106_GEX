#ifndef _FBO_HG_
#define _FBO_HG_

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cFBO
{
public:
	cFBO() : 
		ID(0), 
		colourTexture_0_ID(0),
		depthTexture_ID(0), 
		width(-1), height(-1) {};

	GLuint ID;						// = 0;
	GLuint colourTexture_0_ID;		// = 0;
	/*
	 * Each entry This buffer will consist of 4 unsigned integers, that will store
	 * 1. Object ID (0 if the object is not to be outlined)
	 * 2. Width of the outline
	 * 3. Color ID of the outline. The colors are defined in an array in the fragment shader
	 * 4. Depth value. We can't sample the depth texture, because it is being sampled as a stencil texture
	 *    If it's set to 0, the shader won't sample the info from the texture
	 */
	GLuint outlineInfoBuffer_1_ID = 0;

//	GLuint TBDTexture_1_ID;
//	GLuint TBDTexture_2_ID;

	GLuint depthTexture_ID;		// = 0;
	GLuint stencilTexture_ID = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

	void clearColourBuffer(int bufferindex);
	void clearAllColourBuffers(void);
	void clearDepthBuffer(void);
	void clearStencilBuffer( int clearColour, int mask = 0xFF );

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif
