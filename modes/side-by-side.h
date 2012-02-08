#ifndef H__SIDEBYSIDE
#define H__SIDEBYSIDE

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
void sideBySide_glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
void sideBySide_glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

#endif /* H__SIDEBYSIDE */
