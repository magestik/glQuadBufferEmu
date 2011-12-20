#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Constantes
#define NONE			0
#define MONOSCOPIC		1
#define ANAGLYPH		2
#define SIDEBYSIDE		3
#define INTERLACED		4

// Debug
extern GLboolean DEBUG;

// Config
extern int QuadBufferMode;

// Fonctionnement
extern GLenum QuadBufferCurrent;
extern GLboolean QuadBufferEnabled;

extern int QuadBufferHeight;
extern int QuadBufferWidth;

/* link to transform functions */
void (*wrap_glDrawBuffer) (GLenum mode);

XVisualInfo *(*wrap_glXChooseVisual) (Display *, int, int *);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*wrap_glutInitDisplayMode) (unsigned int displayMode);
void (*wrap_glutReshapeWindow) (int width, int height);

/* link to real functions */
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);

XVisualInfo *(*real_glXChooseVisual) (Display *, int, int *);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

void (*real_glutInitDisplayMode) (unsigned int displayMode);
void (*real_glutReshapeWindow) (int width, int height);
