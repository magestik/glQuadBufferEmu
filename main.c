#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glQuadBufferEmu.h"

#include "./modes/monoscopic.h"
#include "./modes/anaglyph.h"
#include "./modes/interlaced.h"
#include "./modes/side-by-side.h"
#include "./modes/frame-sequential.h"


/* Global var */
unsigned int QuadBufferHeight = 0;
unsigned int QuadBufferWidth = 0;
GLboolean QuadBufferFullscreen = GL_FALSE;

GLenum QuadBufferCurrent = GL_FRONT; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
GLboolean QuadBufferEnabled = GL_FALSE; // GL_TRUE when glXChooseVisual(GLX_STEREO)

void *libGL_handle;
void *libX11_handle;

struct handleName wrap[NB_WRAP_FUNCTIONS] = {
    { glClear, "glClear" },
    { glDrawBuffer, "glDrawBuffer" },
    { glDisable, "glDisable" },
    { glEnable, "glEnable" },
    { glGetBooleanv, "glGetBooleanv" },
    { glGetDoublev, "glGetDoublev" },
    { glGetFloatv, "glGetFloatv" },
    { glGetIntegerv, "glGetIntegerv" },
    { glScissor, "glScissor" },
    { glViewport, "glViewport" },
    { glXChooseFBConfig , "glXChooseFBConfig" },
    { glXChooseVisual, "glXChooseVisual" },
    { glXSwapBuffers, "glXSwapBuffers"},
    { glXGetConfig, "glXGetConfig" },
    { glXGetProcAddress, "glXGetProcAddress" },
    { glXGetProcAddressARB, "glXGetProcAddressARB" },
    { XCreateWindow, "XCreateWindow" },
    { XDestroyWindow, "XDestroyWindow" },
    { XNextEvent, "XNextEvent" },
    { XPeekEvent, "XPeekEvent" },
    { XWindowEvent, "XWindowEvent" }
};




/* Returning our wrap function to dlsym or glXGetProcAdress*/
void *QuadBufferEmuFindFunction(const char *symbol){
    int i = 0;

    while( i < NB_WRAP_FUNCTIONS ) {
        if( !strcmp(wrap[i].symbol, symbol) ) {
            return wrap[i].handle;
        }

        i++;
    }

    return NULL;
}

/* Loading all function we want to wrap */
void QuadBufferEmuLoadLibs(void) {

    libGL_handle = dlopen("libGL.so", RTLD_LAZY);
    if (!libGL_handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }

    libX11_handle = dlopen("libX11.so", RTLD_LAZY);
    if (!libX11_handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }

    real_glClear = dlsym_test(libGL_handle, "glClear");
    real_glDrawBuffer = dlsym_test(libGL_handle, "glDrawBuffer");
    real_glDisable = dlsym_test(libGL_handle, "glDisable");
    real_glEnable = dlsym_test(libGL_handle, "glEnable");
    real_glGetBooleanv = dlsym_test(libGL_handle, "glGetBooleanv");
    real_glGetDoublev = dlsym_test(libGL_handle, "glGetDoublev");
    real_glGetFloatv = dlsym_test(libGL_handle, "glGetFloatv");
    real_glGetIntegerv = dlsym_test(libGL_handle, "glGetIntegerv");
    real_glScissor = dlsym_test(libGL_handle, "glScissor");
    real_glViewport = dlsym_test(libGL_handle, "glViewport");

    real_glXChooseFBConfig = dlsym_test(libGL_handle, "glXChooseFBConfig");
    real_glXChooseVisual = dlsym_test(libGL_handle, "glXChooseVisual");
    real_glXSwapBuffers = dlsym_test(libGL_handle, "glXSwapBuffers");
    real_glXGetConfig = dlsym_test(libGL_handle, "glXGetConfig");
    real_glXGetFBConfigAttrib = dlsym_test(libGL_handle, "glXGetFBConfigAttrib");
    real_glXGetProcAddress = dlsym_test(libGL_handle, "glXGetProcAddress");
    real_glXGetProcAddressARB = dlsym_test(libGL_handle, "glXGetProcAddressARB");

    real_XCreateWindow = dlsym_test(libX11_handle, "XCreateWindow");
    real_XDestroyWindow = dlsym_test(libX11_handle, "XDestroyWindow");
    real_XNextEvent = dlsym_test(libX11_handle, "XNextEvent");
    real_XPeekEvent = dlsym_test(libX11_handle, "XPeekEvent");
    real_XWindowEvent = dlsym_test(libX11_handle, "XWindowEvent");
}

void QuadBufferEmuLoadConf(void) { // FIXME : parse ~/.stereoscopic.conf
    MODE = FRAMESEQUENTIAL;
}

void QuadBufferEmuLoadMode(GLint m) {
    wrap_glClear = NULL;
    wrap_glDrawBuffer = NULL;
    wrap_glDisable = NULL;
    wrap_glEnable = NULL;
    wrap_glGetBooleanv = NULL;
    wrap_glGetDoublev = NULL;
    wrap_glGetFloatv = NULL;
    wrap_glGetIntegerv = NULL;
    wrap_glScissor = NULL;
    wrap_glViewport = NULL;

    wrap_glXChooseFBConfig = NULL;
    wrap_glXChooseVisual = NULL;
    wrap_glXSwapBuffers = NULL;

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
            fprintf(stderr, "Quad-Buffer Stereo Wrapper: Mode not supported !\n");
            exit(1);
    }
}

void QuadBufferEmuInit(void) {
    printf("Quad-Buffer Stereo Wrapper: LOAD\n");

    QuadBufferEmuLoadConf();
    QuadBufferEmuLoadLibs();
    QuadBufferEmuLoadMode(MODE);
}

void QuadBufferEmuExit(void) {
    printf("Quad-Buffer Stereo Wrapper: UNLOAD\n");
}
