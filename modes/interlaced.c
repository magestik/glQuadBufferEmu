#include "../glQuadBufferEmu.h"
#include "../wrappers.h"

#include "interlaced.h"


void initInterlacedMode(void){
	// vertical ou horizontal ?
	
	wrap_glDrawBuffer = interlaced_glDrawBuffer;
}

void interlaced_glDrawBuffer(GLenum mode) {
	// http://www.gali-3d.com/archive/articles/StereoOpenGL/StereoscopicOpenGLTutorial.php
	
	real_glDrawBuffer(mode);
	
	if(QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE); // on laisse passer que le rouge
	} else {
		glClear(GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE); // on laisse passer que le cyan
	}
}
/*
void interlace_stencil(int gliWindowWidth, int gliWindowHeight)
{
	GLint gliY;
	// seting screen-corresponding geometry
	glViewport(0,0,gliWindowWidth,gliWindowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,gliWindowWidth-1,0.0,gliWindowHeight-1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	// clearing and configuring stencil drawing
	glDrawBuffer(GL_BACK);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE); // colorbuffer is copied to stencil
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS,1,1); // to avoid interaction with stencil content
	
	// drawing stencil pattern
	glColor4f(1,1,1,0);	// alfa is 0 not to interfere with alpha tests
	for (gliY=0; gliY<gliWindowHeight; gliY+=2)
	{
		glLineWidth(1);
		glBegin(GL_LINES);
			glVertex2f(0,gliY);
			glVertex2f(gliWindowWidth,gliY);
		glEnd();	
	}
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP); // disabling changes in stencil buffer
	glFlush();
}
*/
