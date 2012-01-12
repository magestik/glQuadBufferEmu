#ifndef H__GLQUADBUFFEREMU
#define H__GLQUADBUFFEREMU

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "wrappers.h"

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


typedef struct handleName {
    void *handle;
    const char *symbol;
} HANDLENAME;

/* User var */
STEREO_MODE MODE;

#define NB_WRAP_FUNCTIONS 21

/* Global var */
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


#endif /* H__GLQUADBUFFEREMU */
