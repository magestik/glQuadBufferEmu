#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Constantes
#define NONE			0
#define MONOSCOPIC		1
#define ANAGLYPH		2
#define SIDEBYSIDE		3
#define INTERLACED		4
#define FRAMESEQUENTIAL	5

/* link to transform functions */
void (*wrap_glClear) (GLbitfield  mask);
void (*wrap_glDrawBuffer) (GLenum mode);
void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*wrap_glGetDoublev) (GLenum pname, GLdouble * params);
void (*wrap_glGetFloatv) (GLenum pname, GLfloat * params);
void (*wrap_glGetIntegerv) (GLenum pname, GLint * params);
void (*wrap_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
void (*wrap_glutReshapeWindow) (int width, int height);

Window (*wrap_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
int (*wrap_XDestroyWindow) (Display *display, Window w);

/* link to real functions */
void (*real_glClear) (GLbitfield  mask);
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*real_glGetDoublev) (GLenum pname, GLdouble * params);
void (*real_glGetFloatv) (GLenum pname, GLfloat * params);
void (*real_glGetIntegerv) (GLenum pname, GLint * params);
void (*real_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*real_glutInitDisplayMode) (unsigned int displayMode);
void (*real_glutReshapeWindow) (int width, int height);

Window (*real_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
int (*real_XDestroyWindow) (Display *display, Window w);
int (*real_XNextEvent) (Display *display, XEvent *event_return);
int (*real_XPeekEvent) (Display *display, XEvent *event_return);
int (*real_XWindowEvent) (Display *display, Window w, long event_mask, XEvent *event_return);
