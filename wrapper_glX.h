#ifndef H__WRAPPER_GLX
#define H__WRAPPER_GLX

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define WRAPPED_FUNCTIONS_GLX
#define X(ret,func,args)\
    ret func args;

#include "wrapped_functions.def"


#define WRAPPED_FUNCTIONS_GLX_GETPROC
#define X(ret,func,args)\
    ret (* func (const GLubyte *procname)) args;

#include "wrapped_functions.def"


/* Real Functions */
#define X(ret,func,args)\
    ret (*real_ ## func) args;

#include "wrapped_functions.def"

#define WRAPPED_FUNCTIONS_GLX_GETPROC
#define X(ret,func,args)\
    ret (*(*real_ ## func )(const GLubyte* procname)) args;

#include "wrapped_functions.def"


/* Wrapped Functions */
#define X(ret,func,args)\
    ret (*wrap_ ## func) args;

#include "wrapped_functions.def"

/* Load and unload GLX wrapper */
void QuadBufferEmuInitGLX (void);
void QuadBufferEmuUnloadGLX (void);

#undef WRAPPED_FUNCTIONS_GLX

#endif /* H_WRAPPER_GLX */
