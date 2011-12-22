#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

// Wrapper for glX - /usr/include/GL/glx.h

XVisualInfo *glXChooseVisual(Display *dpy, int screen, int *attribList){	
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
		QuadBufferEnabled = GL_TRUE;
		fprintf(stderr, "glXChooseVisual(GLX_STEREO)\n");
	} else {
		fprintf(stderr, "glXChooseVisual(.)\n");
	}

    
	if(wrap_glXChooseVisual == NULL || QuadBufferEnabled == GL_FALSE) {
		return real_glXChooseVisual(dpy, screen, wrapped_attribList);
	} else {
		return wrap_glXChooseVisual(dpy, screen, wrapped_attribList);
	}
}

Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx) {
	Bool b;
	//unsigned int v;
	
	fprintf(stderr, "glXMakeCurrent(.)\n");

	b = real_glXMakeCurrent(dpy, drawable, ctx);
	
	if(ctx != NULL){
		//glXQueryDrawable(dpy, drawable, GLX_WIDTH, &v);
		//glXQueryDrawable(dpy, drawable, GLX_HEIGHT, &v);
	}
	
	return b;
}
			    
void glXSwapBuffers(Display * dpy, GLXDrawable drawable){
	if(DEBUG) fprintf(stderr, "glXSwapBuffers(.)\n");
	
	if(wrap_glXSwapBuffers == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glXSwapBuffers(dpy, drawable);
	} else {
		wrap_glXSwapBuffers(dpy, drawable);
	}
}
