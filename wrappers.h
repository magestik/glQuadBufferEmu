#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* Constants */
#define NONE			0
#define MONOSCOPIC		1
#define ANAGLYPH		2
#define SIDEBYSIDE		3
#define INTERLACED		4

/* User var */
extern GLint MODE;
extern GLboolean DEBUG;

/* Global var */
extern unsigned int QuadBufferHeight;
extern unsigned int QuadBufferWidth;
extern GLboolean QuadBufferFullscreen;

extern GLenum QuadBufferCurrent;
extern GLboolean QuadBufferEnabled;

extern void *QuadBufferEmuFindFunction(const char *symbol);

/* Wrap functions */
extern void (*wrap_glClear) (GLbitfield  mask);
extern void (*wrap_glDrawBuffer) (GLenum mode);
extern void (*wrap_glDisable) (GLenum cap);
extern void (*wrap_glEnable) (GLenum cap);
extern void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*wrap_glGetDoublev) (GLenum pname, GLdouble * params);
extern void (*wrap_glGetFloatv) (GLenum pname, GLfloat * params);
extern void (*wrap_glGetIntegerv) (GLenum pname, GLint * params);
extern void (*wrap_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
extern void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

GLXFBConfig* (*wrap_glXChooseFBConfig) (Display *dpy,  int screen, const int * attrib_list, int * nelements);
extern XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern Window (*wrap_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
extern int (*wrap_XDestroyWindow) (Display *display, Window w);

/* Real functions */
extern void (*real_glClear) (GLbitfield  mask);
extern void (*real_glDrawBuffer) (GLenum mode);
extern void (*real_glDisable) (GLenum cap);
extern void (*real_glEnable) (GLenum cap);
extern void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*real_glGetDoublev) (GLenum pname, GLdouble * params);
extern void (*real_glGetFloatv) (GLenum pname, GLfloat * params);
extern void (*real_glGetIntegerv) (GLenum pname, GLint * params);
extern void (*real_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
extern void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

GLXFBConfig* (*real_glXChooseFBConfig) (Display *dpy,  int screen, const int * attrib_list, int * nelements);
extern XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);
extern int (*real_glXGetConfig) (Display *dpy, XVisualInfo *vis, int attrib, int *value);
extern int (*real_glXGetFBConfigAttrib) (Display *dpy,  GLXFBConfig config, int attribute,  int *value);
extern void (*(*real_glXGetProcAddress)(const GLubyte *procname))( void );
extern void (*(*real_glXGetProcAddressARB)(const GLubyte *procName))( void );

extern Window (*real_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
extern int (*real_XDestroyWindow) (Display *display, Window w);
extern int (*real_XNextEvent) (Display *display, XEvent *event_return);
extern int (*real_XPeekEvent) (Display *display, XEvent *event_return);
extern int (*real_XWindowEvent) (Display *display, Window w, long event_mask, XEvent *event_return);
