#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

// Wrapper for glut - /usr/include/GL/freeglut_std.h

void glutInitDisplayMode (unsigned int displayMode){
	
	if(displayMode & GLUT_STEREO) {
		fprintf(stderr, "glutInitDisplayMode(GLUT_STEREO)\n");
	} else {
		fprintf(stderr, "glutInitDisplayMode(.)\n");
	}
	
	if(displayMode & GLUT_DOUBLE) {
		QuadBufferCurrent = GL_BACK;
	}
	
	displayMode = displayMode & ~GLUT_STEREO;
	
	if(wrap_glutInitDisplayMode == NULL) {
		return real_glutInitDisplayMode(displayMode);
	} else {
		return wrap_glutInitDisplayMode(displayMode);
	}
}
