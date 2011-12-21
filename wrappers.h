#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* Wrap functions */
extern void (*wrap_glClear) (GLbitfield  mask);
extern void (*wrap_glDrawBuffer) (GLenum mode);
extern void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

extern XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
extern void (*wrap_glutReshapeWindow) (int width, int height);

/* Real functions */
extern void (*real_glClear) (GLbitfield  mask);
extern void (*real_glDrawBuffer) (GLenum mode);
extern void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
extern void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

extern XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
extern Bool (*real_glXMakeCurrent) (Display *dpy, GLXDrawable drawable, GLXContext ctx);
extern void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

extern void (*real_glutInitDisplayMode) (unsigned int displayMode);
extern void (*real_glutReshapeWindow) (int width, int height);
