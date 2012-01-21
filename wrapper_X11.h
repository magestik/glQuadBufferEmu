#ifndef H__WRAPPER_X11
#define H__WRAPPER_X11

#define WRAPPED_FUNCTIONS_X11

/* Real functions */
#define X(ret,func,args)\
    ret (*real_ ## func) args;

#include "wrapped_functions.def"

/* Wrapped functions */
#define X(ret,func,args)\
    ret (*wrap_ ## func) args;

#include "wrapped_functions.def"

/* Init and unload X11 wrapper */
void QuadBufferEmuInitX11 (void);
void QuadBufferEmuUnloadX11 (void);

#undef WRAPPED_FUNCTIONS_X11

#endif /* H__WRAPPER_X11 */
