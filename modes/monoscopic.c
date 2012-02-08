#include "../glQuadBufferEmu.h"
#include "monoscopic.h"

// Buffers: 9/9


void initMonoscopicMode (void)
{
    QBState.monoscopic.view = GL_RIGHT;
    QBState.monoscopic.buffer = GL_RIGHT;
    wrap_glDrawBuffer = monoscopic_glDrawBuffer;
}

/* TODO: Implement the choice of left or right view via QBState.monoscopic.view
 */
void monoscopic_glDrawBuffer (GLenum mode)
{
    switch (QBState.current)
    {
    case GL_FRONT_LEFT:
    case GL_BACK_LEFT:
    case GL_LEFT:
        if (QBState.monoscopic.buffer == GL_LEFT)
        {
            real_glDrawBuffer (mode);
        }
        else
        {
            real_glDrawBuffer (GL_NONE);
        }
        break;

    case GL_FRONT_RIGHT:
    case GL_BACK_RIGHT:
    case GL_RIGHT:
        if (QBState.monoscopic.buffer == GL_RIGHT)
        {
            real_glDrawBuffer (mode);
        }
        else
        {
            real_glDrawBuffer (GL_NONE);
        }
        break;

    default: // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
        real_glDrawBuffer(mode);
    }
}
