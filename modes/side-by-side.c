#include <stdio.h>
#include <stdlib.h>

#include "../glQuadBufferEmu.h"
#include "../wrappers.h"

#include "side-by-side.h"


void initSideBySideMode(void){
	// Fullscreen (TODO: glX fullscreen)
	//glutFullScreen(); 
	
	// For HDMI 1.4a display
	//system("xrandr -s 1920x2205 -r 24");
	//system("xrandr -s 1280x1470 -r 60");
	
	// TODO : viewport + GenLock for Nvidia 3D Vision
	
	// TODO : activate 3D mode for active shutter glasses
	
	setCorrectViewport();

	wrap_glDrawBuffer = sideBySide_glDrawBuffer;
	wrap_glViewport = sideBySide_glViewport;
	wrap_glutReshapeWindow = sideBySide_glutReshapeWindow;
}

void setCorrectViewport(void){

	/* Top/Bottom */
	//setTopBottomViewport();
	
	/* Left/Right */
	//setLeftRightViewport();
	
	/* frame-packing */
	setFramePackedViewport();
}

void setTopBottomViewport(void){
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth;
	leftViewport[3] = QuadBufferHeight/2;
	
	fprintf(stderr, "leftViewport(%d, %d, %d, %d)\n", leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
	
	rightViewport[0] = 0;
	rightViewport[1] = QuadBufferHeight/2;
	rightViewport[2] = QuadBufferWidth;
	rightViewport[3] = QuadBufferHeight/2;
	
	fprintf(stderr, "rightViewport(%d, %d, %d, %d)\n", rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);
}

void setLeftRightViewport(void){	
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth/2;
	leftViewport[3] = QuadBufferHeight;
	
	rightViewport[0] = QuadBufferWidth/2;
	rightViewport[1] = 0;
	rightViewport[2] = QuadBufferWidth/2;
	rightViewport[3] = QuadBufferHeight;
}

void setFramePackedViewport(void){
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth;
	leftViewport[3] = QuadBufferHeight/2;
	
	rightViewport[0] = 0;
	rightViewport[1] = (QuadBufferHeight/2)+(QuadBufferHeight/49); // the draw are MUST be in fullscreen and in the correct resolution for this to work correctly
	rightViewport[2] = QuadBufferWidth;
	rightViewport[3] = QuadBufferHeight/2;
	
	if( glIsEnabled(GL_SCISSOR_TEST) ) glDisable(GL_SCISSOR_TEST);
	glScissor(0, QuadBufferHeight/2, QuadBufferWidth, (QuadBufferHeight/49));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if( !glIsEnabled(GL_SCISSOR_TEST) ) glEnable(GL_SCISSOR_TEST);
}

void sideBySide_glDrawBuffer(GLenum mode) {

	if( glIsEnabled(GL_SCISSOR_TEST) ) glDisable(GL_SCISSOR_TEST);

	real_glDrawBuffer(mode);
	
	if (QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT) {
		real_glViewport(leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
		glScissor(leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
	} else {
		real_glViewport(rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);
		glScissor(rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);
	}
	
	if( !glIsEnabled(GL_SCISSOR_TEST) ) glEnable(GL_SCISSOR_TEST);
}

void sideBySide_glutReshapeWindow(int width, int height) {
	setCorrectViewport();
	real_glutReshapeWindow(width, height);
}

void sideBySide_glViewport(GLint x, GLint y, GLsizei width, GLsizei height){
	if (QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT) {
		if( x < leftViewport[2] && y < leftViewport[3]){
			real_glViewport(leftViewport[0]+x, leftViewport[1]+y, min(width, leftViewport[2]-x), min(height, leftViewport[3]-y));  // leftViewport[2], leftViewport[3]
			if(DEBUG) fprintf(stderr, "ERROR: left Viewport(%d, %d, %d, %d)\n", leftViewport[0]+x, leftViewport[1]+y, min(width, leftViewport[2]-x), min(height, leftViewport[3]-y));
		} else {
			fprintf(stderr, "ERROR: left Viewport(%d, %d, %d, %d)\n", leftViewport[0]+x, leftViewport[1]+y, min(width, leftViewport[2]-x), min(height, leftViewport[3]-y));
		}
	} else {
		if( x < rightViewport[2] && y < rightViewport[3]){
			real_glViewport(rightViewport[0]+x, rightViewport[1]+y, min(width, rightViewport[2]-x), min(height, rightViewport[3]-y));  // leftViewport[2], leftViewport[3]
			if(DEBUG) fprintf(stderr, "ERROR: right Viewport(%d, %d, %d, %d)\n", rightViewport[0]+x, rightViewport[1]+y, min(width, rightViewport[2]-x), min(height, rightViewport[3]-y));
		} else {
			fprintf(stderr, "ERROR: right Viewport(%d, %d, %d, %d)\n", rightViewport[0]+x, rightViewport[1]+y, min(width, rightViewport[2]-x), min(height, rightViewport[3]-y));
		}
	}	
	
}
