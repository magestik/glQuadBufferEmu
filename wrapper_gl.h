#ifndef H__WRAPPER_GL
#define H__WRAPPER_GL

#define WRAPPED_FUNCTIONS_GL

/* Real functions */
#define X(ret,func,args)\
    ret (*real_ ## func) args;

#include "wrapped_functions.def"

/* Wrap functions */
#define X(ret,func,args)\
    ret (*wrap_ ## func) args;

#include "wrapped_functions.def"

/* Special case, glXGetProcAdress */
#define WRAPPED_FUNCTIONS_GLX_GETPROC
#define X(ret,func,args)\
    ret (*(*real_ ## func )(const GLubyte* procname)) args;

#include "wrapped_functions.def"

#undef WRAPPED_FUNCTIONS_GL

/* Load and unload GL wrapper */
void QuadBufferEmuInitGL (void);
void QuadBufferEmuUnloadGL (void);

#endif /* H__WRAPPER_GL */
