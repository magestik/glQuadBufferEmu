#include "wrappers.h"

static void *libGL_handle = NULL;
static void *libX11_handle = NULL;

#define WRAPPED_FUNCTIONS_GL

void QuadBufferEmuInitGL(void)
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

#undef WRAPPED_FUNCTIONS_GL

void QuadBufferEmuUnloadGL (void)
{
    dlclose (libGL_handle);
}

#define WRAPPED_FUNCTIONS_X11

void QuadBufferEmuInitX11 (void)
{
    libX11_handle = dlsym_open_lib ("libX11.so");

    /* Init wrappers */
    #define X(ret,func,args)\
        wrap_ ## func = NULL;

    #include "wrapped_functions.def"

    /* tell wrapper_dlsym to wrap GL functions */
    #define X(ret,func,args)\
        dlsym_add_wrap (func, #func);

    #include "wrapped_functions.def"

    /* Link real functions */
    #define X(ret,func,args)\
        real_ ## func = dlsym_test (libX11_handle, #func);

    #include "wrapped_functions.def"
}

#undef WRAPPED_FUNCTIONS_X11

void QuadBufferEmuUnloadX11 (void)
{
    dlclose (libX11_handle);
}


