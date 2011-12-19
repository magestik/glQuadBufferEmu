#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"

#include "./modes/monoscopic.h"
#include "./modes/anaglyph.h"
#include "./modes/interlaced.h"
#include "./modes/side-by-side.h"

/* globals */
void *libGL_handle, *libGLUT_handle;

int QuadBufferMode;
GLenum QuadBufferCurrent;
GLboolean DEBUG;

/* dsym with error checking */
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
}

void QuadBufferEmuLoadConf(void){
	QuadBufferCurrent = GL_FRONT; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
	
	// FIXME : parse ~/.stereoscopic.conf
	QuadBufferMode = 0;
	DEBUG = GL_FALSE;
}

void QuadBufferEmuLoadMode(void)
{
	wrap_glDrawBuffer = NULL;
	wrap_glXChooseVisual = NULL;
	wrap_glXSwapBuffers = NULL;
	wrap_glutInitDisplayMode = NULL;
	
	switch(QuadBufferMode){
		default:
			initAnaglyphMode();
	}
}

void QuadBufferEmuInit(void)
{
	QuadBufferEmuLoadLibs();
	QuadBufferEmuLoadConf();
	QuadBufferEmuLoadMode();
}


