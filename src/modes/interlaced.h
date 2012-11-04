#ifndef H__INTERLACED
#define H__INTERLACED

#include <GL/gl.h>
#include <GL/glx.h>

typedef enum
{
    VERTICAL,
    HORIZONTAL,
    CHECKERBOARD,
    N_INTERLACED_MODE
} INTERLACED_MODE;

typedef struct interlaceState
{
    INTERLACED_MODE mode; /* this isn't implemented yet */

} INTERLACED_STATE;

void interlace_stencil(int gliWindowWidth, int gliWindowHeight);
void interlaced_glDrawBuffer(GLenum mode);
XVisualInfo *interlaced_glXChooseVisual(Display *dpy, int screen, int *attribList);
void interlaced_glutInitDisplayMode(unsigned int displayMode);

void initInterlacedMode(void);

#endif /* H__INTERLACED */
