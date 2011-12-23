#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define min(x1,x2) ((x1) > (x2) ? (x2):(x1))
#define max(x1,x2) ((x1) < (x2) ? (x2):(x1))

// Constantes
#define NONE			0
#define MONOSCOPIC		1
#define ANAGLYPH		2
#define SIDEBYSIDE		3
#define INTERLACED		4

// Config utilisateur
extern GLboolean DEBUG;
extern GLint MODE;

// Fonctionnement
extern GLenum QuadBufferCurrent;
extern GLboolean QuadBufferEnabled;

extern unsigned int QuadBufferHeight;
extern unsigned int QuadBufferWidth;

/* Wrap functions */
extern void (*wrap_glClear) (GLbitfield  mask);
extern void (*wrap_glDrawBuffer) (GLenum mode);
extern void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*wrap_glGetDoublev) (GLenum pname, GLdouble * params);
extern void (*wrap_glGetFloatv) (GLenum pname, GLfloat * params);
extern void (*wrap_glGetIntegerv) (GLenum pname, GLint * params);
extern void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

extern XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
extern void (*wrap_glutReshapeWindow) (int width, int height);

extern Window (*wrap_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
extern int (*wrap_XDestroyWindow) (Display *display, Window w);

/* Real functions */
extern void (*real_glClear) (GLbitfield  mask);
extern void (*real_glDrawBuffer) (GLenum mode);
extern void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*real_glGetDoublev) (GLenum pname, GLdouble * params);
extern void (*real_glGetFloatv) (GLenum pname, GLfloat * params);
extern void (*real_glGetIntegerv) (GLenum pname, GLint * params);
extern void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

extern XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*real_glutInitDisplayMode) (unsigned int displayMode);
extern void (*real_glutReshapeWindow) (int width, int height);

extern Window (*real_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
extern int (*real_XDestroyWindow) (Display *display, Window w);
extern int (*real_XNextEvent) (Display *display, XEvent *event_return);
extern int (*real_XPeekEvent) (Display *display, XEvent *event_return);
extern int (*real_XWindowEvent) (Display *display, Window w, long event_mask, XEvent *event_return);
