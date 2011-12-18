#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void initMonoscopicMode(void);

GLenum monoscopicBuffer;

/* Transformation functions */
void monoscopic_glDrawBuffer(GLenum mode);

