#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define min(x1,x2) ((x1) > (x2) ? (x2):(x1))

// Constantes
#define NONE			0
#define MONOSCOPIC		1
#define ANAGLYPH		2
#define SIDEBYSIDE		3
#define INTERLACED		4

// Debug
extern GLboolean DEBUG;

// Config
extern int QuadBufferMode;

// Fonctionnement
extern GLenum QuadBufferCurrent;
extern GLboolean QuadBufferEnabled;

extern unsigned int QuadBufferHeight;
extern unsigned int QuadBufferWidth;

/* link to transform functions */
void (*wrap_glClear) (GLbitfield  mask);
void (*wrap_glDrawBuffer) (GLenum mode);
void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
void (*wrap_glutReshapeWindow) (int width, int height);

/* link to real functions */
void (*real_glClear) (GLbitfield  mask);
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
Bool (*real_glXMakeCurrent) (Display *dpy, GLXDrawable drawable, GLXContext ctx);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*real_glutInitDisplayMode) (unsigned int displayMode);
void (*real_glutReshapeWindow) (int width, int height);
