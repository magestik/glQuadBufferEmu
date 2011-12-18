#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

/* link to real functions */
void (*real_glDrawBuffer) (GLenum mode);

XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*real_glutInitDisplayMode) (unsigned int displayMode);


/* globals */
void *libGL_handle, *libGLUT_handle;

GLenum current_buffer;

/* dsym with check */
void *dlsym_test(void *lib, char *name)
{
    char *error;
    void *function = dlsym(lib, name);
	
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(1);
    }

    return function;
}

/* Init functions */
void QuadBufferEmuLoadLibs(void){
	fprintf(stderr, "QuadBufferEmuInit\n");
	
	libGL_handle = dlopen("/usr/lib/i386-linux-gnu/libGL.so", RTLD_LAZY);
	if (!libGL_handle) {
		fputs(dlerror(), stderr);
		exit(1);
    }
    
	libGLUT_handle = dlopen("/usr/lib/libglut.so", RTLD_LAZY);
	if (!libGLUT_handle) {
		fputs(dlerror(), stderr);
		exit(1);
    }
    
	real_glDrawBuffer = dlsym_test(libGL_handle, "glDrawBuffer");
	
    real_glXChooseVisual = dlsym_test(libGL_handle, "glXChooseVisual");
	real_glXSwapBuffers = dlsym_test(libGL_handle, "glXSwapBuffers");
	
	real_glutInitDisplayMode = dlsym_test(libGLUT_handle, "glutInitDisplayMode");
	real_glutKeyboardFunc = dlsym_test(libGLUT_handle, "glutKeyboardFunc"); 
}

void QuadBufferEmuLoadConf(void){
	// FIXME : parse ~/.stereoscopic.conf
	current_buffer = GL_FRONT; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
}

void QuadBufferEmuInit(void)
{
	QuadBufferEmuLoadLibs();
	QuadBufferEmuLoadConf();
}
