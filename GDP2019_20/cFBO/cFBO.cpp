#include "cFBO.h"


// Calls shutdown(), then init()
bool cFBO::reset(int width, int height, std::string &error)
{
	if ( ! this->shutdown() )
	{
		error = "Could not shutdown";
		return false;
	}

	return this->init( width, height, error );
}

bool cFBO::shutdown(void)
{
	glDeleteTextures( 1, &(this->colourTexture_0_ID) );
	glDeleteTextures(1, &(this->outlineInfoBuffer_1_ID));
	
	glDeleteTextures( 1, &(this->depthTexture_ID) );

	glDeleteFramebuffers( 1, &(this->ID) );

	return true;
}


bool cFBO::init( int width, int height, std::string &error )
{
	this->width = width;
	this->height = height;

//	glCreateFramebuffers(1, &( this->ID ) );	// GL 4.5		//g_FBO
	glGenFramebuffers( 1, &( this->ID ) );		// GL 3.0
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

	//************************************************************
	//     ___     _              
	//    / __|___| |___ _  _ _ _ 
	//   | (__/ _ \ / _ \ || | '_|
	//    \___\___/_\___/\_,_|_|  
	//                            
	// Create the colour buffer (texture)
	glGenTextures(1, &(this->colourTexture_0_ID ) );		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->colourTexture_0_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,		// 8 bits per colour
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,			// Colour goes to #0
		this->colourTexture_0_ID, 0);

	//     ___        _   _ _              ___        __       
	//    / _ \ _   _| |_| (_)_ __   ___  |_ _|_ __  / _| ___  
	//   | | | | | | | __| | | '_ \ / _ \  | || '_ \| |_ / _ \ 
	//   | |_| | |_| | |_| | | | | |  __/  | || | | |  _| (_) |
	//    \___/ \__,_|\__|_|_|_| |_|\___| |___|_| |_|_|  \___/ 
	//
	glGenTextures(1, &(this->outlineInfoBuffer_1_ID));		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->outlineInfoBuffer_1_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32UI,		// 4 unsigned 32 bit integers
		this->width,				// g_FBO_SizeInPixes
		this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT1,			// outline info goes to #1
		this->outlineInfoBuffer_1_ID, 0);

	//    ___           _   _      ___       __  __         
	//   |   \ ___ _ __| |_| |_   | _ )_  _ / _|/ _|___ _ _ 
	//   | |) / -_) '_ \  _| ' \  | _ \ || |  _|  _/ -_) '_|
	//   |___/\___| .__/\__|_||_| |___/\_,_|_| |_| \___|_|  
	//            |_|                                       
	glGenTextures(1, &( this->depthTexture_ID ));			//g_FBO_depthTexture
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8,
				   this->width,		//g_FBO_SizeInPixes
				   this->height);
	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		this->depthTexture_ID, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//    ___ _               _ _   ___       __  __         
	//   / __| |_ ___ _ _  __(_) | | _ )_  _ / _|/ _|___ _ _ 
	//   \__ \  _/ -_) ' \/ _| | | | _ \ || |  _|  _/ -_) '_|
	//   |___/\__\___|_||_\__|_|_| |___/\_,_|_| |_| \___|_|  
	//
	// We need to do 2 because we need to set up different ways to
	// sample them
	//glGenTextures(1, &(this->stencilTexture_ID));			//g_FBO_depthTexture
	//glBindTexture(GL_TEXTURE_2D, this->stencilTexture_ID);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_STENCIL_INDEX16,	//GL_DEPTH32F_STENCIL8,
	//	this->width,		//g_FBO_SizeInPixes
	//	this->height);
	//glFramebufferTexture(GL_FRAMEBUFFER,
	//	GL_STENCIL_ATTACHMENT,
	//	this->stencilTexture_ID, 0);

// ***************************************************************

	static const GLenum draw_bufers[] = 
	{
		GL_COLOR_ATTACHMENT0, // Color
		GL_COLOR_ATTACHMENT1 // outline info
	};
	glDrawBuffers(2, draw_bufers);		// There are 2 outputs now

	
	// ***************************************************************
	// ADD ONE MORE THING...
	bool bFrameBufferIsGoodToGo = true; 

	switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true; 
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
//	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_UNSUPPORTED:
		error = "GL_FRAMEBUFFER_UNSUPPORTED";
	default:
		bFrameBufferIsGoodToGo = false; 
		break;
	}//switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

void cFBO::clearColourBuffer(int bufferindex)
{
	glViewport(0, 0, this->width, this->height);
	GLfloat	zero = 0.0f;
	glClearBufferfv(GL_COLOR, bufferindex, &zero);

	return;
}


void cFBO::clearBuffers(bool bClearColour, bool bClearDepth)
{
	glViewport(0, 0, this->width, this->height);
	GLfloat	zero = 0.0f;
	GLfloat zero_x3[3] = { zero, zero, zero };
	GLfloat zero_x4[4] = { zero, zero, zero, zero };
	GLfloat one = 1.0f;
	if ( bClearColour )
	{
		glClearBufferfv(GL_COLOR, 0, zero_x3);		// Colour
		glClearBufferfv(GL_COLOR, 1, zero_x4);		// Outline info
	}
	glClearBufferfv(GL_DEPTH, 0, &one);		// Depth is normalized 0.0 to 1.0f
	if ( bClearDepth )
	{
	}
	// If buffer is GL_STENCIL, drawbuffer must be zero, and value points to a 
	//  single value to clear the stencil buffer to. Masking is performed in the 
	//  same fashion as for glClearStencil. Only the *iv forms of these commands 
	//  should be used to clear stencil buffers; be used to clear stencil buffers; 
	//  other forms do not accept a buffer of GL_STENCIL.
	
	// 
	glStencilMask(0xFF);

	{	// Clear stencil
		//GLint intZero = 0;
		//glClearBufferiv(GL_STENCIL, 0, &intZero );
		glClearBufferfi( GL_DEPTH_STENCIL, 
						 0,		// Must be zero
						 1.0f,	// Clear value for depth
						 0 );	// Clear value for stencil
	}

	return;
}


int cFBO::getMaxColourAttachments(void)
{
	//  void glGetIntegerv(GLenum pname,
	// 				       GLint * data);
	
	int maxColourAttach = 0;
	glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach );

	return maxColourAttach;
}

int cFBO::getMaxDrawBuffers(void)
{
	int maxDrawBuffers = 0;
	glGetIntegerv( GL_MAX_DRAW_BUFFERS, &maxDrawBuffers );
	
	return maxDrawBuffers;
}
