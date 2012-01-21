#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"
#include "glQuadBufferEmu.h"

// Wrapper for GL - /usr/include/GL/gl.h

static void *libGL_handle = NULL;


#define WRAPPED_FUNCTIONS_GL

void QuadBufferEmuInitGL (void)
{
    libGL_handle = dlsym_open_lib("libGL.so");

    /* Init wrappers */
    #define X(ret,func,args)\
        wrap_ ## func = NULL;

    #include "wrapped_functions.def"

    /* tell wrapper_dlsym to wrap GL functions */
    #define X(ret,func,args)\
        dlsym_add_wrap (func, #func);

    #include "wrapped_functions.def"

    #define X(ret,func,args)\
        dlsym_add_wrap (func, #func);
    #define WRAPPED_FUNCTIONS_GLX_GETPROC

    #include "wrapped_functions.def"

    /* Link real functions */
    #define X(ret,func,args)\
        real_ ## func = dlsym_test (libGL_handle, #func);

    #include "wrapped_functions.def"

    #define X(ret,func,args)\
        real_ ## func = dlsym_test (libGL_handle, #func);
    #define WRAPPED_FUNCTIONS_GLX_GETPROC

    #include "wrapped_functions.def"
}


void QuadBufferEmuUnloadGL (void)
{
    dlclose (libGL_handle);
}

