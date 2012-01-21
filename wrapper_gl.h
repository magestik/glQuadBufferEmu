#ifndef H__WRAPPER_GL
#define H__WRAPPER_GL

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define WRAPPED_FUNCTIONS_GL

#define X(ret,func,args)\
    ret func args;

#include "wrapped_functions.def"


/* Special case, glXGetProcAdress */
#define WRAPPED_FUNCTIONS_GLX_GETPROC
#define X(ret,func,args)\
    ret (* func (const GLubyte *procname)) args;

#include "wrapped_functions.def"


/* Real functions */
#define X(ret,func,args)\
    ret (*real_ ## func) args;

#include "wrapped_functions.def"


/* Special case, glXGetProcAdress */
#define WRAPPED_FUNCTIONS_GLX_GETPROC
#define X(ret,func,args)\
    ret (*(*real_ ## func )(const GLubyte* procname)) args;

#include "wrapped_functions.def"



/* link to transform functions */
#define X(ret,func,args)\
    ret (*wrap_ ## func) args;

#include "wrapped_functions.def"

#undef WRAPPED_FUNCTIONS_GL

/* Load and unload GL wrapper */
void QuadBufferEmuInitGL (void);
void QuadBufferEmuUnloadGL (void);

#endif /* H__WRAPPER_GL */
