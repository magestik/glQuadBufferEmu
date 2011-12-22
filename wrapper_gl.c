#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

// Wrapper for GL - /usr/include/GL/gl.h

void glClear(GLbitfield mask) { // http://www.opengl.org/sdk/docs/man/xhtml/glClear.xml
	if(DEBUG) fprintf(stderr, "glClear(.)\n");
	
	if(wrap_glClear == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glClear(mask);
	} else {
		wrap_glClear(mask);
	}
}

void glDrawBuffer(GLenum mode) { // http://www.opengl.org/sdk/docs/man/xhtml/glDrawBuffer.xml
	if(DEBUG) fprintf(stderr, "glDrawBuffer(.)\n");
	
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
	
	if(wrap_glDrawBuffer == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glDrawBuffer(mode);
	} else {
		wrap_glDrawBuffer(mode);
	}
}

/* http://www.opengl.org/sdk/docs/man/xhtml/glGet.xml */

void glGetBooleanv(GLenum pname, GLboolean * params) {
	if(DEBUG) fprintf(stderr, "glGetBooleanv(.)\n");
	
	switch(pname) {
		case GL_STEREO:
			*params = GL_TRUE;
		break;
		
		default:
			if(wrap_glGetBooleanv == NULL || QuadBufferEnabled == GL_FALSE) {
				real_glGetBooleanv(pname, params);
			} else {
				wrap_glGetBooleanv(pname, params);
			}
	}
}

void glGetDoublev(GLenum pname, GLdouble * params) {
	if(DEBUG) fprintf(stderr, "glGetDoublev(.)\n");
	
	if(wrap_glGetDoublev == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glGetDoublev(pname, params);
	} else {
		wrap_glGetDoublev(pname, params);
	}
}

void glGetFloatv(GLenum pname, GLfloat * params) {
	if(DEBUG) fprintf(stderr, "glGetFloatv(.)\n");
	
	if(wrap_glGetFloatv == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glGetFloatv(pname, params);
	} else {
		wrap_glGetFloatv(pname, params);
	}
}

void glGetIntegerv(GLenum pname, GLint * params) {
	if(DEBUG) fprintf(stderr, "glGetIntegerv(.)\n");
	
	if(wrap_glGetIntegerv == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glGetIntegerv(pname, params);
	} else {
		wrap_glGetIntegerv(pname, params);
	}
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) { // http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
	if(DEBUG) fprintf(stderr, "glViewport(%d, %d, %d, %d)\n", x, y, width, height);
	
	if(wrap_glViewport == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glViewport(x, y, width, height);
	} else {
		wrap_glViewport(x, y, width, height); // Side-By-Side
	}
}
