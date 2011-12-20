#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* Wrap functions */
extern void (*wrap_glDrawBuffer) (GLenum mode);

extern XVisualInfo *(*wrap_glXChooseVisual) (Display *, int, int *);
extern void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
extern void (*wrap_glutReshapeWindow) (int width, int height);

/* Real functions */
extern void (*real_glDrawBuffer) (GLenum mode);
extern void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);

extern XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*real_glutInitDisplayMode) (unsigned int displayMode);
extern void (*real_glutReshapeWindow) (int width, int height);
