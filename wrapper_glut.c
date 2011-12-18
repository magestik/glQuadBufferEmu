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
	
	displayMode = displayMode & ~GLUT_STEREO;
	return real_glutInitDisplayMode(displayMode);
}
