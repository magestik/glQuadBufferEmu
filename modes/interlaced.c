#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "interlaced.h"

void initInterlacedMode(void){
	// vertical ou horizontal ?
interlace_stencil(QuadBufferWidth, QuadBufferHeight);
	wrap_glDrawBuffer = interlaced_glDrawBuffer;
	wrap_glXChooseVisual = interlaced_glXChooseVisual;
	wrap_glutInitDisplayMode = interlaced_glutInitDisplayMode;
}

void interlace_stencil(int gliWindowWidth, int gliWindowHeight) {
	int y;

	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
                                      
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	
	glEnable(GL_STENCIL_TEST);  

	for(y=0;y<gliWindowHeight;y+=2) {
		glBegin(GL_LINES);
		glVertex3f(0, y, 0);
		glVertex3f(gliWindowWidth, y, 0);
		glEnd();
	}
	
/*
	for(int x=0;x<kContextWidth;x+=2) {
		glBegin(GL_LINES);
		glVertex3f(x, 0, 0);
		glVertex3f(x, kContextHeight, 0);
		glEnd();
	}
*/
                                        
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );         
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void interlaced_glDrawBuffer(GLenum mode) {
	// http://www.gali-3d.com/archive/articles/StereoOpenGL/StereoscopicOpenGLTutorial.php
	if( glIsEnabled(GL_STENCIL_TEST) ) glDisable(GL_STENCIL_TEST);
	
	if(QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT || QuadBufferCurrent == GL_LEFT){
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 1); // draws if stencil <> 1
	} else {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 1); // draws if stencil <> 0
	}
	
	real_glDrawBuffer(mode);
}

XVisualInfo *interlaced_glXChooseVisual(Display *dpy, int screen, int *attribList){
	XVisualInfo *r;
	int i = 0;
	
	while(attribList[i] != None) {
		if(attribList[i] == GLX_STENCIL_SIZE){
			fprintf(stderr, "glXChooseVisual(GLX_STENCIL_SIZE) incompatible with interlaced display mode !\n");
			exit(1);
		}
		i++;
	}

	attribList[i] = GLX_STENCIL_SIZE;
    attribList[i+1] = 1;
    attribList[i+2] = None;

	r = real_glXChooseVisual(dpy, screen, attribList);
	return r;
}

void interlaced_glutInitDisplayMode(unsigned int displayMode) {
	real_glutInitDisplayMode(displayMode | GLUT_STENCIL);
}
