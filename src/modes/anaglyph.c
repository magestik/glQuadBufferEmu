#include "glQuadBufferEmu.h"
#include "anaglyph.h"

// Buffers: 9/9 *OK*

GLboolean ColorMasks[N_COLOR_CODE][3] =
{              /*  RED       GREEN     BLUE        */
    /* WHITE   */ {GL_FALSE, GL_FALSE, GL_FALSE},
    /* BLUE    */ {GL_FALSE, GL_FALSE, GL_TRUE},
    /* GREEN   */ {GL_FALSE, GL_TRUE,  GL_FALSE},
    /* RED     */ {GL_FALSE, GL_TRUE,  GL_TRUE},
    /* CYAN    */ {GL_TRUE,  GL_FALSE, GL_FALSE},
    /* MAGENTA */ {GL_TRUE,  GL_FALSE, GL_TRUE},
    /* YELLOW  */ {GL_TRUE,  GL_TRUE,  GL_FALSE},
    /* BLACK   */ {GL_TRUE,  GL_TRUE,  GL_TRUE}
};

void initAnaglyphMode (void)
{
    QBState.anaglyph.leftMask = ColorMasks[QBState.anaglyph.leftColor];
    QBState.anaglyph.rightMask = ColorMasks[QBState.anaglyph.rightColor];

    wrap_glDrawBuffer = anaglyph_glDrawBuffer;
}

void anaglyph_setColorMask (GLenum eye, COLOR_CODE color)
{
    if (eye == GL_RIGHT) {
        QBState.anaglyph.rightMask = ColorMasks[color];
        QBState.anaglyph.rightColor = color;
    } else if (eye == GL_LEFT) {
        QBState.anaglyph.leftMask = ColorMasks[color];
        QBState.anaglyph.leftColor = color;
    }
}

/* GL WRAP FUNCTIONS */

void anaglyph_glDrawBuffer (GLenum mode)
{
    real_glDrawBuffer (mode);

    if (QBState.current == GL_BACK_LEFT ||  QBState.current == GL_FRONT_LEFT ||  QBState.current == GL_LEFT) {

        real_glClear (GL_DEPTH_BUFFER_BIT); // | GL_COLOR_BUFFER_BIT
        glColorMask (QBState.anaglyph.leftMask[0], QBState.anaglyph.leftMask[1], QBState.anaglyph.leftMask[2], GL_TRUE);

    } else if (QBState.current == GL_BACK_RIGHT ||  QBState.current == GL_FRONT_RIGHT ||  QBState.current == GL_RIGHT) {

        real_glClear (GL_DEPTH_BUFFER_BIT);
        glColorMask (QBState.anaglyph.rightMask[0], QBState.anaglyph.rightMask[1], QBState.anaglyph.rightMask[2], GL_TRUE);

    } else { // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
}
