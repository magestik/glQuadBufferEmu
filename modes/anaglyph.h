#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Constantes
#define WHITE		0
#define BLUE		1
#define GREEN		2
#define RED			3
#define CYAN		4
#define MAGENTA		5
#define YELLOW		6
#define BLACK		7

void initAnaglyphMode(void);
void anaglyph_getColorMask(int c, GLboolean *tab);

GLenum monoscopicBuffer;

GLboolean anaglyphLeft[3];
GLboolean anaglyphRight[3];

/* Transformation functions */
void anaglyph_glDrawBuffer(GLenum mode);
