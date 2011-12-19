#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Debug
extern GLboolean DEBUG;

// Config
extern int QuadBufferMode;

// Fonctionnement
extern GLenum QuadBufferCurrent;


/* link to transform functions */
void (*wrap_glDrawBuffer) (GLenum mode);
void (*wrap_glVertex3f) (GLfloat  x,  GLfloat  y,  GLfloat  z);

XVisualInfo *(*wrap_glXChooseVisual) (Display *, int, int *);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*wrap_glutInitDisplayMode) (unsigned int displayMode);

/* link to real functions */
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glVertex3f) (GLfloat  x,  GLfloat  y,  GLfloat  z);

XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*real_glutInitDisplayMode) (unsigned int displayMode);
