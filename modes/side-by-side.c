#include <stdio.h>
#include <stdlib.h>

#include "../glQuadBufferEmu.h"
#include "../wrappers.h"

#include "side-by-side.h"


void initSideBySideMode(void){

	/* TODO :
	 * Passer en plein écran
	 * Régler la résolution (compatible HDMI 1.4 ou double viewport pour Nvidia 3D Vision avec GenLock)
	 * Activer le module noyau de contrôle des lunettes actives (Nvidia 3D Vision)
	 * */

	setCorrectViewport();

	wrap_glDrawBuffer = sideBySide_glDrawBuffer;
	wrap_glutReshapeWindow = sideBySide_glutReshapeWindow;
}

void setCorrectViewport(void){

	/* Top/Bottom */
	setTopBottomViewport();
	
	/* Left/Right */
	//setLeftRightViewport();
	
	/* frame-packing */
	//setFramePackedViewport();
}

void setTopBottomViewport(void){
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth;
	leftViewport[3] = QuadBufferHeight/2;
	
	fprintf(stderr, "Viewport(%d, %d, %d, %d)\n", leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
	
	rightViewport[0] = 0;
	rightViewport[1] = QuadBufferHeight/2;
	rightViewport[2] = QuadBufferWidth;
	rightViewport[3] = QuadBufferHeight/2;
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
	rightViewport[1] = (QuadBufferHeight/2)+(QuadBufferHeight/49); // works in fullscreen in the good resolution
	rightViewport[2] = QuadBufferWidth;
	rightViewport[3] = QuadBufferHeight/2;
}

void sideBySide_glDrawBuffer(GLenum mode) {
	
	if (QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT) {
		glViewport(leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
	} else {
		glViewport(rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);	
	}
	
	real_glDrawBuffer(mode);
}

void sideBySide_glutReshapeWindow(int width, int height) {
	setCorrectViewport();
	real_glutReshapeWindow(width, height);
}
