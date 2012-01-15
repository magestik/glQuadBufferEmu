#ifndef H__WRAPPER_GL
#define H__WRAPPER_GL

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

extern void *libGL_handle;

#define WRAPPED_FUNCTIONS_GL

#define X(func,args)\
    void func args;

#include "wrapped_functions.def"

/* Real functions */
#define X(func,args)\
    void (*real_ ## func) args;

#include "wrapped_functions.def"

/* link to transform functions */
#define X(func,args)\
    void (*wrap_ ## func) args;

#include "wrapped_functions.def"

#undef WRAPPED_FUNCTIONS_GL

/* Load and unload GL wrapper */
void QuadBufferEmuInitGL (void);
void QuadBufferEmuUnloadGL (void);

#endif /* H__WRAPPER_GL */
