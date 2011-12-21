#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

// Wrapper for GL - /usr/include/GL/gl.h

void glClear(GLbitfield mask) {
	if(wrap_glClear == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glClear(mask);
	} else {
		wrap_glClear(mask);
	}
}

void glDrawBuffer(GLenum mode) { // http://www.opengl.org/sdk/docs/man/xhtml/glDrawBuffer.xml
	
	QuadBufferCurrent = mode;
	
	switch(mode){
		case GL_FRONT_LEFT: 
		case GL_FRONT_RIGHT: 
		case GL_FRONT:
			mode = GL_FRONT;
		break;
		
		case GL_BACK_LEFT:
		case GL_BACK_RIGHT:
		case GL_BACK:
			mode = GL_BACK;
		break;
		
		case GL_NONE:
			mode = GL_NONE;
		break;
		
		case GL_FRONT_AND_BACK:
			mode = GL_FRONT_AND_BACK;
		break;
		
		default:
			/*
			GL_LEFT  = GL_FRONT_LEFT + GL_BACK_LEFT
			GL_RIGHT = GL_FRONT_RIGHT +  GL_BACK_RIGHT

			GL_AUXi
			*/
			
			mode = GL_BACK;
			fprintf(stderr, "QuadBufferEmu Error : buffer mode not yet supported (using GL_BACK instead)\n");
	}
	
	if(DEBUG) fprintf(stderr, "glDrawBuffer(.)\n");
	
	if(wrap_glDrawBuffer == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glDrawBuffer(mode);
	} else {
		wrap_glDrawBuffer(mode);
	}
}

void glGetBooleanv(GLenum pname, GLboolean * params) {	// http://www.opengl.org/sdk/docs/man/xhtml/glGetBooleanv.xml
	if (pname == GL_STEREO) {
		*params = GL_TRUE;
	} else {
		real_glGetBooleanv(pname, params);
	}
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height){
	if(wrap_glDrawBuffer == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glViewport(x, y, width, height);
	} else {
		wrap_glViewport(x, y, width, height); // Side-By-Side
	}
}
