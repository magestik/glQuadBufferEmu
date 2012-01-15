#ifndef H__GLQUADBUFFEREMU
#define H__GLQUADBUFFEREMU

/*#include "wrappers.h"*/

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "./modes/monoscopic.h"
#include "./modes/anaglyph.h"
#include "./modes/interlaced.h"
#include "./modes/side-by-side.h"
#include "./modes/frame-sequential.h"

/* the different modes supported by the emulator. */
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


typedef struct
{
    unsigned int        height;
    unsigned int        width;
    GLboolean           fullscreen;

    /* The initial value is GL_FRONT for single-buffered contexts,
     * and GL_BACK for double-buffered contexts. */
    GLenum              current;

    /* GL_TRUE when glXChooseVisual(GLX_STEREO) */
    GLboolean           enabled;

    /* the current mode that the emulator is running on */
    STEREO_MODE         mode;

    /* The states of the different view modes */
    MONOSCOPIC_STATE       monoscopic;
    ANAGLYPH_STATE         anaglyph;
    INTERLACE_STATE        interlaced;
    SIDEBYSIDE_STATE       sidebyside;
    FRAMESEQUENTIAL_STATE  framesequential;

} QuadBufferState;


/* The state of our quad buffer emulator */
QuadBufferState QBState;

/* Init and unload functions */
void QuadBufferEmuLoadLibs (void);
void QuadBufferEmuLoadConf (void);
void QuadBufferEmuLoadMode (void);
void QuadBufferEmuInit (void);
void QuadBufferEmuExit (void);


#endif /* H__GLQUADBUFFEREMU */
