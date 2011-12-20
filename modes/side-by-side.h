#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void initSideBySideMode(void);

void setCorrectViewport(void);
void setLeftRightViewport(void);
void setTopBottomViewport(void);
void setFramePackedViewport(void);

int leftViewport[4];
int rightViewport[4];

/* Transformation functions */
void sideBySide_glDrawBuffer(GLenum mode);
void sideBySide_glutReshapeWindow(int width, int height);
