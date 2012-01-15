#ifndef H__SIDEBYSIDE
#define H__SIDEBYSIDE

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
#include <X11/extensions/xf86vmode.h>
*/

typedef enum {
    SBS_TOPBOTTOM,
    SBS_LEFTRIGHT,
    SBS_FRAMEPACKED,
    N_SIDEBYSIDE_MODE
} SIDEBYSIDE_MODE;

typedef struct sideBySideState
{
    int   leftViewport[4];
    int   rightViewport[4];
    int   ratio[2];

    SIDEBYSIDE_MODE mode;

} SIDEBYSIDE_STATE;



/* Side by side mode functions */
void initSideBySideMode(void);
void setCorrectViewport(void);
void setLeftRightViewport(void);
void setTopBottomViewport(void);
void setFramePackedViewport(void);


/* Transformation functions */
void sideBySide_glDrawBuffer(GLenum mode);
void sideBySide_glGetIntegerv(GLenum pname, GLint * params);
void sideBySide_glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

int sideBySide_glutCreateWindow( const char* title );
void sideBySide_glutReshapeWindow(int width, int height);

Window sideBySide_XCreateWindow
        (Display *display, Window parent, int x, int y, unsigned int width,
         unsigned int height, unsigned int border_width, int depth,
         unsigned int class, Visual *visual, unsigned long valuemask,
         XSetWindowAttributes *attributes);

int sideBySide_XDestroyWindow (Display *display, Window w);

#endif /* H__SIDEBYSIDE */
