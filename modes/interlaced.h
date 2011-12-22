#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void initInterlacedMode(void);
void interlace_stencil(int gliWindowWidth, int gliWindowHeight);

void interlaced_glDrawBuffer(GLenum mode);
XVisualInfo *interlaced_glXChooseVisual(Display *dpy, int screen, int *attribList);
void interlaced_glutInitDisplayMode(unsigned int displayMode);
