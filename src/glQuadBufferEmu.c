#include "glQuadBufferEmu.h"
#include "wrappers.h"

/* The Quad-Buffer emulator default state */
QuadBufferState QBState =
{
    0,
    0,
    GL_FALSE,
    GL_FRONT,
    GL_FALSE,
    FRAMESEQUENTIAL,
    {0, 0},
    {0, RED, CYAN, NULL, NULL},
    {VERTICAL},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0}, SBS_LEFTRIGHT},
    {0, NV3DVISION, NULL},
    {
        initMonoscopicMode,
        initAnaglyphMode,
        initSideBySideMode,
        initInterlacedMode,
        initFrameSequentialMode
    }
};

void QuadBufferEmuInitMode (void)
{
    if (QBState.mode >= N_STEREO_MODE) {
        FATAL_ERROR ("Quad-Buffer Stereo Wrapper: Mode not supported !")
    } else {
        QBState.init_mode[QBState.mode] ();
    }
}

/* initialize the lib and all function we want to wrap */
void QuadBufferEmuInit(void)
{
    QuadBufferEmuLoadConf();
    QuadBufferEmuInitGL();
    QuadBufferEmuInitX11();

    atexit(QuadBufferEmuExit);

    QuadBufferEmuInitMode();
}

void QuadBufferEmuExit(void)
{
    #ifdef DEBUG
        fprintf (stderr, "Exiting ...\n");
    #endif
    
    QuadBufferEmuUnloadGL();
    QuadBufferEmuUnloadX11();
}
