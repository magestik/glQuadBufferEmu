#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* Wrap functions */
extern void (*wrap_glDrawBuffer) (GLenum mode);
extern void (*wrap_glVertex3f) (GLfloat  x,  GLfloat  y,  GLfloat  z);

extern XVisualInfo *(*wrap_glXChooseVisual) (Display *, int, int *);
extern void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*wrap_glutInitDisplayMode) (unsigned int displayMode);

/* Real functions */
extern void (*real_glDrawBuffer) (GLenum mode);
extern void (*real_glVertex3f) (GLfloat  x,  GLfloat  y,  GLfloat  z);

extern XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*real_glutInitDisplayMode) (unsigned int displayMode);
