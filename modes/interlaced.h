#ifndef H__INTERLACED
#define H__INTERLACED

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef enum
{
    VERTICAL,
    HORIZONTAL,
    CHECKERBOARD
} INTERLACE_MODE;

typedef struct interlaceState
{
    INTERLACE_MODE mode; /* this isn't implemented yet */

} INTERLACE_STATE;

void initInterlacedMode(void);
void interlace_stencil(int gliWindowWidth, int gliWindowHeight);

void interlaced_glDrawBuffer(GLenum mode);
XVisualInfo *interlaced_glXChooseVisual(Display *dpy, int screen, int *attribList);
void interlaced_glutInitDisplayMode(unsigned int displayMode);

#endif /* H__INTERLACED */
