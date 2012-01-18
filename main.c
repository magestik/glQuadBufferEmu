#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glQuadBufferEmu.h"
#include "wrappers.h"

/* The QuadBuffer emulator state */
QuadBufferState QBState =
{
    0,
    0,
    GL_FALSE,
    GL_FRONT,
    GL_FALSE,
    FRAMESEQUENTIAL,  // FRAMESEQUENTIAL,
    {0, 0},
    {0, RED, CYAN, NULL, NULL},
    {VERTICAL},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0}, SBS_LEFTRIGHT},
    {0, NV3DVISION, NULL}
};


/* Loading and unload all function we want to wrap */
void QuadBufferEmuLoadLibs (void)
{
    QuadBufferEmuInitGL ();
    QuadBufferEmuInitGLX ();
    QuadBufferEmuInitX11 ();
}

void QuadBufferEmuUnloadLibs (void)
{
    QuadBufferEmuUnloadGL ();
    QuadBufferEmuUnloadGLX ();
    QuadBufferEmuUnloadX11 ();
}

/* TODO : parse ~/.config/glQuadBufferEmu/.glQuadBufferEmu.conf */
void QuadBufferEmuLoadConf (void)
{
    /* set QBState */
}

void QuadBufferEmuLoadMode ()
{
    switch (QBState.mode) {
		
		case FRAMESEQUENTIAL:
			initFrameSequentialMode();
        break;

		case INTERLACED:
			initInterlacedMode();
        break;

		case SIDEBYSIDE:
			initSideBySideMode();
        break;

		case ANAGLYPH:
			initAnaglyphMode();
        break;

		case MONOSCOPIC:
			initMonoscopicMode();
        break;

		case NONE:
        default:
			FATAL_ERROR ("Quad-Buffer Stereo Wrapper: Mode not supported !");
    }
}

void QuadBufferEmuInit(void)
{
    QuadBufferEmuLoadConf();
    QuadBufferEmuLoadLibs();
    QuadBufferEmuLoadMode();
    atexit (QuadBufferEmuExit);
}

void QuadBufferEmuExit(void)
{
    #ifdef DEBUG
        fprintf (stderr, "Exiting ...\n");
    #endif
    QuadBufferEmuUnloadLibs ();
}
