#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

// Wrapper for glX - /usr/include/GL/glx.h

XVisualInfo *glXChooseVisual(Display *dpy, int screen, int *attribList){
	XVisualInfo *result;
	
	int wrapped_attribList[60]; // 60 is enough ?
	int requested = 0;
	
	int i = 0;
	
	while(attribList[i] != None) {
		
		if(attribList[i] == GLX_STEREO) {
			requested++;
		} else {
			wrapped_attribList[i-requested] = attribList[i];
		}

		i++;
	}

	wrapped_attribList[i-requested] = None;
	
	if(requested > 0) {
		fprintf(stderr, "glXChooseVisual(GLX_STEREO)\n");
	} else {
		fprintf(stderr, "glXChooseVisual(.)\n");
	}

    result = real_glXChooseVisual(dpy, screen, wrapped_attribList);
    return (result);	
}

void glXSwapBuffers(Display * dpy, GLXDrawable drawable){
	//fprintf(stderr, "glXSwapBuffers(.)\n");
	real_glXSwapBuffers(dpy, drawable);
}
