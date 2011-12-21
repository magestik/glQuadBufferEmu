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

unsigned int QuadBufferHeight;
unsigned int QuadBufferWidth;

GLenum QuadBufferCurrent;
GLboolean QuadBufferEnabled;
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
    
    real_glClear = dlsym_test(libGL_handle, "glClear");
	real_glDrawBuffer = dlsym_test(libGL_handle, "glDrawBuffer");
	real_glGetBooleanv = dlsym_test(libGL_handle, "glGetBooleanv");
	real_glViewport = dlsym_test(libGL_handle, "glViewport");
	
    real_glXChooseVisual = dlsym_test(libGL_handle, "glXChooseVisual");
    real_glXMakeCurrent = dlsym_test(libGL_handle, "glXMakeCurrent");
	real_glXSwapBuffers = dlsym_test(libGL_handle, "glXSwapBuffers");

	real_glutInitDisplayMode = dlsym_test(libGLUT_handle, "glutInitDisplayMode");
	real_glutReshapeWindow = dlsym_test(libGLUT_handle, "glutReshapeWindow");
}

void QuadBufferEmuLoadConf(void){
	QuadBufferHeight = 300;
	QuadBufferWidth = 300;
	
	QuadBufferCurrent = GL_FRONT; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
	QuadBufferEnabled = GL_FALSE;
	
	// FIXME : parse ~/.stereoscopic.conf
	QuadBufferMode = SIDEBYSIDE;
	DEBUG = GL_FALSE;
}

void QuadBufferEmuLoadMode(void)
{
	wrap_glClear = NULL;
	wrap_glDrawBuffer = NULL;
	wrap_glGetBooleanv = NULL;
	wrap_glViewport = NULL;
	
	wrap_glXChooseVisual = NULL;
	wrap_glXSwapBuffers = NULL;
	
	wrap_glutInitDisplayMode = NULL;
	wrap_glutReshapeWindow = NULL;
	
	switch(QuadBufferMode){
		case INTERLACED:
			initInterlacedMode();
		break;
		
		case SIDEBYSIDE:
			initSideBySideMode();
		break;
			
		case ANAGLYPH:
			initAnaglyphMode();
		break;
				
		case MONOSCOPIC:
			initMonoscopicMode();
		break;
		
		case NONE:
		default:
			fprintf(stderr, "Mode not supported !\n");
			exit(1);
	}
}

void QuadBufferEmuInit(void)
{
	QuadBufferEmuLoadLibs();
	QuadBufferEmuLoadConf();
	QuadBufferEmuLoadMode();
}
