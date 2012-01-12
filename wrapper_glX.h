#ifndef H__WRAPPER_GLX
#define H__WRAPPER_GLX

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glQuadBufferEmu.h"

GLXFBConfig *glXChooseFBConfig(Display *dpy, int screen,const int *attrib_list, int *nelements);
XVisualInfo *glXChooseVisual(Display *dpy, int screen, int *attribList);
int glXGetConfig(Display *dpy, XVisualInfo *vis, int attrib, int *value);
int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
void (*glXGetProcAddress(const GLubyte *procname)) (void);
void (*glXGetProcAddressARB(const GLubyte *procname)) (void);
void glXSwapBuffers(Display * dpy, GLXDrawable drawable);


/* Real Functions */
GLXFBConfig* (*real_glXChooseFBConfig) (Display *dpy, int screen, const int * attrib_list, int * nelements);
XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);
int (*real_glXGetConfig) (Display *dpy, XVisualInfo *vis, int attrib, int *value);
int (*real_glXGetFBConfigAttrib) (Display *dpy, GLXFBConfig config, int attribute, int *value);
void (*(*real_glXGetProcAddress) (const GLubyte *procname))( void );
void (*(*real_glXGetProcAddressARB)(const GLubyte *procName))( void );

/* Wrapped Functions */
GLXFBConfig* (*wrap_glXChooseFBConfig) (Display *dpy,  int screen, const int * attrib_list,  int * nelements);
XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

#endif /* H_WRAPPER_GLX */
