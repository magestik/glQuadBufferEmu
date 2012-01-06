#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "side-by-side.h"

// Buffers: 6/9 (TODO: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)

void initSideBySideMode(void){
	// TODO : Hardware page-flipping for active shutter glasses ( X server viewport + GenLock )
	
	setCorrectViewport();

	wrap_glDrawBuffer = sideBySide_glDrawBuffer;
	wrap_glGetIntegerv = sideBySide_glGetIntegerv;
	wrap_glViewport = sideBySide_glViewport;
	
	wrap_glutReshapeWindow = sideBySide_glutReshapeWindow;
	
	wrap_XCreateWindow = sideBySide_XCreateWindow;
}

void setCorrectViewport(void) {

	/* Top/Bottom */
	setTopBottomViewport();
	
	/* Left/Right */
	//setLeftRightViewport();
	
	/* frame-packing */
	//setFramePackedViewport();
}

void setTopBottomViewport(void) {
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth;
	leftViewport[3] = QuadBufferHeight/2;
	
	rightViewport[0] = 0;
	rightViewport[1] = QuadBufferHeight/2;
	rightViewport[2] = QuadBufferWidth;
	rightViewport[3] = QuadBufferHeight/2;
}

void setLeftRightViewport(void) {	
	leftViewport[0] = 0;
	leftViewport[1] = 0;
	leftViewport[2] = QuadBufferWidth/2;
	leftViewport[3] = QuadBufferHeight;
	
	rightViewport[0] = QuadBufferWidth/2;
	rightViewport[1] = 0;
	rightViewport[2] = QuadBufferWidth/2;
	rightViewport[3] = QuadBufferHeight;
}

void setFramePackedViewport(void) {
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

/* GL */

void sideBySide_glDrawBuffer(GLenum mode) {

	if( glIsEnabled(GL_SCISSOR_TEST) ) glDisable(GL_SCISSOR_TEST);

	real_glDrawBuffer(mode);
	
	if(QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT || QuadBufferCurrent == GL_LEFT){
		real_glViewport(leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
		glScissor(leftViewport[0], leftViewport[1], leftViewport[2], leftViewport[3]);
	
	} else if(QuadBufferCurrent == GL_BACK_RIGHT || QuadBufferCurrent == GL_FRONT_RIGHT || QuadBufferCurrent == GL_RIGHT){
		real_glViewport(rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);
		glScissor(rightViewport[0], rightViewport[1], rightViewport[2], rightViewport[3]);
		
	} else { // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
		fprintf(stderr, "WARNING: writing in (GL_FRONT, GL_BACK or GL_FRONT_AND_BACK) not yet supported in side-by-side\n");
	}
	
	if( !glIsEnabled(GL_SCISSOR_TEST) ) glEnable(GL_SCISSOR_TEST);
}

void sideBySide_glGetIntegerv(GLenum pname, GLint * params) {
	real_glGetIntegerv(pname, params);
	
	if(pname == GL_VIEWPORT) {
		if (QuadBufferCurrent == GL_BACK_RIGHT || QuadBufferCurrent == GL_FRONT_RIGHT || QuadBufferCurrent == GL_RIGHT) {
			params[0] -= rightViewport[0];
			params[1] -= rightViewport[1];
		}
	}
}

void sideBySide_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	if (QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT || QuadBufferCurrent == GL_LEFT) {
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

/* GLUT */
void sideBySide_glutReshapeWindow(int width, int height) {
	setCorrectViewport();
	real_glutReshapeWindow(width, height);
}

/* X11 */
Window sideBySide_XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {

	// XCreateWindow called by :
	// - the program itself if the program is not using GLUT
	// - glutCreateWindow if the program is using GLUT
	
	/*
	XF86VidModeModeInfo **modes;
	int modeNum, bestMode;
	*/
	
	int scrnum = DefaultScreen(display); // FIXME : Always DefaultScreen ?

	// Go to HDMI 1.4 resolution	(1920x2205@24 or 1280x1470@60)
	// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
	/*
	XF86VidModeGetAllModeLines(display, scrnum, &modeNum, &modes); 
	desktopMode = *modes[0];
	bestMode = 0;
	for (i = 0; i < modeNum; i++) {                                                                    
		if ( modes[i]->hdisplay == 1920 && modes[i]->vdisplay == 2205 ) { // the best
			bestMode = i;                                                   
		}
		if ( modes[i]->hdisplay == 1280 && modes[i]->vdisplay == 1470 && bestMode == 0) {
			bestMode = i;
		}
	} 
	
	XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
	XF86VidModeSetViewPort(display, screen, 0, 0);
	width = modes[bestMode]->hdisplay;
	height = modes[bestMode]->vdisplay;
	XFree(modes); 	
	*/
	
	x = 0; y = 0;
	width = DisplayWidth(display, scrnum );
	height = DisplayHeight(display, scrnum );
	
	QuadBufferHeight = height;
	QuadBufferWidth = width;
	setCorrectViewport();
	
	attributes->override_redirect = True;	
	valuemask = valuemask | CWOverrideRedirect;
	
	return real_XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
}

int sideBySide_XDestroyWindow (Display *display, Window w) {
	// Get the resolution in default mode
	/*
	XF86VidModeSwitchToMode(display, screen, &desktopMode);
	XF86VidModeSetViewPort(display, screen, 0, 0);
	*/
	
	return real_XDestroyWindow(display, w);
}
