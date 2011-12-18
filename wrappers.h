#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

extern void (*real_glDrawBuffer) (GLenum mode);

extern XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*real_glutInitDisplayMode) (unsigned int displayMode);
extern void (*real_glutKeyboardFunc) (void (*func)(unsigned char key, int x, int y));
