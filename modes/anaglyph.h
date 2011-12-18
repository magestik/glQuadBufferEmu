#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void initAnaglyphMode(void);

GLenum monoscopicBuffer;

/* Transformation functions */
void anaglyph_glDrawBuffer(GLenum mode);
