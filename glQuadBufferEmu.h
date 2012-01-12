#ifndef H__GLQUADBUFFEREMU
#define H__GLQUADBUFFEREMU

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef enum
{
    NONE,
    MONOSCOPIC,
    ANAGLYPH,
    SIDEBYSIDE,
    INTERLACED,
    FRAMESEQUENTIAL,
    N_STEREO_MODE
} STEREO_MODE;

#define NB_WRAP_FUNCTIONS 21

typedef struct handleName {
    void *handle;
    const char *symbol;
} HANDLENAME;

/* User var */
STEREO_MODE MODE;

/* Global var */ /* gaffe à l'implementation dans un header */
extern HANDLENAME wrap[NB_WRAP_FUNCTIONS];

extern unsigned int QuadBufferHeight;
extern unsigned int QuadBufferWidth;
extern GLboolean QuadBufferFullscreen;

extern GLenum QuadBufferCurrent; // The initial value is GL_FRONT for single-buffered contexts, and GL_BACK for double-buffered contexts.
extern GLboolean QuadBufferEnabled; // GL_TRUE when glXChooseVisual(GLX_STEREO)

/* Init functions */
void *QuadBufferEmuFindFunction(const char *symbol);
void QuadBufferEmuLoadLibs(void);
void QuadBufferEmuLoadConf(void);
void QuadBufferEmuLoadMode(GLint m);

void QuadBufferEmuInit(void);
void QuadBufferEmuExit(void);


/* dlsym wrap */
extern void *dlsym_test(void *lib, const char *name);


/* link to transform functions */
void (*wrap_glClear) (GLbitfield  mask);
void (*wrap_glDrawBuffer) (GLenum mode);
void (*wrap_glDisable) (GLenum  cap);
void (*wrap_glEnable) (GLenum  cap);
void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*wrap_glGetDoublev) (GLenum pname, GLdouble * params);
void (*wrap_glGetFloatv) (GLenum pname, GLfloat * params);
void (*wrap_glGetIntegerv) (GLenum pname, GLint * params);
void (*wrap_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

GLXFBConfig* (*wrap_glXChooseFBConfig) (Display *dpy,  int screen, const int * attrib_list,  int * nelements);
XVisualInfo* (*wrap_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*wrap_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);

Window (*wrap_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
int (*wrap_XDestroyWindow) (Display *display, Window w);

/* link to real functions */
void (*real_glClear) (GLbitfield  mask);
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glDisable) (GLenum  cap);
void (*real_glEnable) (GLenum  cap);
void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*real_glGetDoublev) (GLenum pname, GLdouble * params);
void (*real_glGetFloatv) (GLenum pname, GLfloat * params);
void (*real_glGetIntegerv) (GLenum pname, GLint * params);
void (*real_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

GLXFBConfig* (*real_glXChooseFBConfig) (Display *dpy,  int screen, const int * attrib_list, int * nelements);
XVisualInfo* (*real_glXChooseVisual) (Display *dpy, int screen, int *attribList);
void (*real_glXSwapBuffers) (Display * dpy, GLXDrawable drawable);
int (*real_glXGetConfig) (Display *dpy, XVisualInfo *vis, int attrib, int *value);
int (*real_glXGetFBConfigAttrib) (Display *dpy,  GLXFBConfig config, int attribute,  int *value);
void (*(*real_glXGetProcAddress) (const GLubyte *procname))( void );
void (*(*real_glXGetProcAddressARB)(const GLubyte *procName))( void );

Window (*real_XCreateWindow) (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
int (*real_XDestroyWindow) (Display *display, Window w);
int (*real_XNextEvent) (Display *display, XEvent *event_return);
int (*real_XPeekEvent) (Display *display, XEvent *event_return);
int (*real_XWindowEvent) (Display *display, Window w, long event_mask, XEvent *event_return);

#endif /* H__GLQUADBUFFEREMU */
