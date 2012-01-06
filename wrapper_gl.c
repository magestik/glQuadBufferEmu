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
	char *bname = "";
	GLboolean wrap = GL_TRUE;
	QuadBufferCurrent = mode;

	switch(mode){
		// GL_ FRONT
		case GL_FRONT:
			bname = "GL_FRONT";
			mode = GL_FRONT;
		break;
		
		case GL_FRONT_LEFT:
			bname = "GL_FRONT_LEFT";
			mode = GL_FRONT;
		break;
		
		case GL_FRONT_RIGHT:
			bname = "GL_FRONT_RIGHT";
			mode = GL_FRONT;
		break;

		// GL_BACK
		case GL_BACK:
			bname = "GL_BACK";
			mode = GL_BACK;
		break;
		
		case GL_BACK_LEFT:
			bname = "GL_BACK_LEFT";
			mode = GL_BACK;
		break;
		
		case GL_BACK_RIGHT:
			bname = "GL_BACK_RIGHT";
			mode = GL_BACK;
		break;

		// GL_FRONT_AND_BACK
		case GL_FRONT_AND_BACK: // ALL 4 BUFFERS
			bname = "GL_FRONT_AND_BACK";
			mode = GL_FRONT_AND_BACK;
		break;
		
		// GL_LEFT  = GL_FRONT_LEFT + GL_BACK_LEFT
		case GL_LEFT: 
			bname = "GL_LEFT";
			mode = GL_FRONT_AND_BACK;
		break;
		
		// GL_RIGHT = GL_FRONT_RIGHT + GL_BACK_RIGHT
		case GL_RIGHT: 
			bname = "GL_RIGHT";
			mode = GL_FRONT_AND_BACK;
		break;

		case GL_NONE:
			bname = "GL_NONE";
			wrap = GL_FALSE;
		break;
		
		default: // GL_AUXi
			bname = "GL_AUXi"; // we don't need to know i value
			wrap = GL_FALSE;
	}
	
	if(DEBUG) fprintf(stderr, "glDrawBuffer(%s)\n", bname);
	
	if(wrap_glDrawBuffer == NULL || QuadBufferEnabled == GL_FALSE || wrap == GL_FALSE) {
		real_glDrawBuffer(mode);
	} else {
		wrap_glDrawBuffer(mode);
	}
}

/* http://www.opengl.org/sdk/docs/man/xhtml/glEnable.xml */

void glDisable(GLenum cap) {
	if(DEBUG) fprintf(stderr, "glDisable(.)\n");

	if(wrap_glDisable == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glDisable(cap);
	} else {
		wrap_glDisable(cap);
	}
}

void glEnable(GLenum cap) {
	if(DEBUG) fprintf(stderr, "glEnable(.)\n");

	if(wrap_glEnable == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glEnable(cap);
	} else {
		wrap_glEnable(cap);
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
	
	switch(pname) {
		case GL_DRAW_BUFFER:
			*params = QuadBufferCurrent;
		break;

		default:
			if(wrap_glGetIntegerv == NULL || QuadBufferEnabled == GL_FALSE) {
				real_glGetIntegerv(pname, params);
			} else {
				wrap_glGetIntegerv(pname, params);
			}
	}
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) { // http://www.opengl.org/sdk/docs/man/xhtml/glScissor.xml
	if(DEBUG) fprintf(stderr, "glScissor(%d, %d, %d, %d)\n", x, y, width, height);

	if(wrap_glScissor == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glScissor(x, y, width, height);
	} else {
		wrap_glScissor(x, y, width, height); // Side-By-Side
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
