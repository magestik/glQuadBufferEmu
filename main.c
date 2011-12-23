#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "glQuadBufferEmu.h"

#include "./modes/monoscopic.h"
#include "./modes/anaglyph.h"
#include "./modes/interlaced.h"
#include "./modes/side-by-side.h"
#include "./modes/frame-sequential.h"

/* globals */
void *libGL_handle, *libGLUT_handle, *libX11_handle;

unsigned int QuadBufferHeight;
unsigned int QuadBufferWidth;

GLenum QuadBufferCurrent;
GLboolean QuadBufferEnabled;

GLint MODE;
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

	libX11_handle = dlopen("/usr/lib/i386-linux-gnu/libX11.so.6", RTLD_LAZY);
	if (!libX11_handle) {
		fputs(dlerror(), stderr);
		exit(1);
	}

	real_glClear = dlsym_test(libGL_handle, "glClear");
	real_glDrawBuffer = dlsym_test(libGL_handle, "glDrawBuffer");
	real_glGetBooleanv = dlsym_test(libGL_handle, "glGetBooleanv");
	real_glGetDoublev = dlsym_test(libGL_handle, "glGetDoublev");
	real_glGetFloatv = dlsym_test(libGL_handle, "glGetFloatv");
	real_glGetIntegerv = dlsym_test(libGL_handle, "glGetIntegerv");
	real_glScissor = dlsym_test(libGL_handle, "glScissor");
	real_glViewport = dlsym_test(libGL_handle, "glViewport");

	real_glXChooseVisual = dlsym_test(libGL_handle, "glXChooseVisual");
	real_glXSwapBuffers = dlsym_test(libGL_handle, "glXSwapBuffers");

	real_glutInitDisplayMode = dlsym_test(libGLUT_handle, "glutInitDisplayMode");
	real_glutReshapeWindow = dlsym_test(libGLUT_handle, "glutReshapeWindow");

	real_XCreateWindow = dlsym_test(libX11_handle, "XCreateWindow");
	real_XDestroyWindow = dlsym_test(libX11_handle, "XDestroyWindow");
	real_XNextEvent = dlsym_test(libX11_handle, "XNextEvent");
	real_XPeekEvent = dlsym_test(libX11_handle, "XPeekEvent");
	real_XWindowEvent = dlsym_test(libX11_handle, "XWindowEvent");
}

void QuadBufferEmuLoadConf(void){
	QuadBufferHeight = 300;
	QuadBufferWidth = 300;

	QuadBufferCurrent = GL_FRONT; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
	QuadBufferEnabled = GL_FALSE;

	// FIXME : parse ~/.stereoscopic.conf
	MODE = FRAMESEQUENTIAL; // <- NE PAS UTILISER LE MODE SIDEBYSIDE POUR LE MOMENT !!!
	DEBUG = GL_FALSE;
}

void QuadBufferEmuLoadMode(GLint m)
{
	wrap_glClear = NULL;
	wrap_glDrawBuffer = NULL;
	wrap_glGetBooleanv = NULL;
	wrap_glGetDoublev = NULL;
	wrap_glGetFloatv = NULL;
	wrap_glGetIntegerv = NULL;
	wrap_glScissor = NULL;
	wrap_glViewport = NULL;

	wrap_glXChooseVisual = NULL;
	wrap_glXSwapBuffers = NULL;

	wrap_glutInitDisplayMode = NULL;
	wrap_glutReshapeWindow = NULL;

	switch(m) {
		case FRAMESEQUENTIAL:
			initFrameSequentialMode();
		break;

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
	QuadBufferEmuLoadMode(MODE);
}
