#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

// Wrapper for GL - /usr/include/GL/gl.h

void glDrawBuffer(GLenum mode)
{ 
	
	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawBuffer.xml
	
	current_buffer = mode;
	
	/*
	GL_NONE
	
	GL_FRONT = GL_FRONT_LEFT + GL_FRONT_RIGHT
	GL_BACK  = GL_BACK_LEFT + GL_BACK_RIGHT
	
	GL_LEFT  = GL_FRONT_LEFT + GL_BACK_LEFT
	GL_RIGHT = GL_FRONT_RIGHT +  GL_BACK_RIGHT

	GL_FRONT_AND_BACK = GL_LEFT + GL_RIGHT

	GL_AUXi
	*/
	
	real_glDrawBuffer(mode);
}
