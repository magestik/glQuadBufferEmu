#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

// Wrapper for glut - /usr/include/GL/freeglut_std.h

void glutInitDisplayMode (unsigned int displayMode){
	
	if(displayMode & GLUT_STEREO) {
		QuadBufferEnabled = GL_TRUE;
		fprintf(stderr, "glutInitDisplayMode(GLUT_STEREO)\n");
	} else {
		fprintf(stderr, "glutInitDisplayMode(.)\n");
	}
	
	if(displayMode & GLUT_DOUBLE) {
		QuadBufferCurrent = GL_BACK;
	}
	
	displayMode = displayMode & ~GLUT_STEREO;
	
	if(wrap_glutInitDisplayMode == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glutInitDisplayMode(displayMode);
	} else {
		wrap_glutInitDisplayMode(displayMode);
	}
}

void glutReshapeWindow( int width, int height ){
	QuadBufferWidth = width;
	QuadBufferHeight = height;
	
	fprintf(stderr, "glutReshapeWindow(%d, %d)\n", width, height);
	
	if(wrap_glutReshapeWindow == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glutReshapeWindow(width, height);
	} else {
		wrap_glutReshapeWindow(width, height);
	}
}
