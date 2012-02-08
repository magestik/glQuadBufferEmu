#include "../glQuadBufferEmu.h"
#include "interlaced.h"

// Buffers: 6/9 (TODO: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)

void initInterlacedMode (void)
{
    // vertical ou horizontal ?
    interlace_stencil (QBState.width, QBState.height);

    wrap_glDrawBuffer = interlaced_glDrawBuffer;
    wrap_glXChooseVisual = interlaced_glXChooseVisual;
}

void interlace_stencil (int gliWindowWidth, int gliWindowHeight)
{
    int y;

    glEnable (GL_STENCIL_TEST);

    glClearStencil (0);
    glClear (GL_STENCIL_BUFFER_BIT);
    glStencilFunc (GL_ALWAYS, 1, 1);
    glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);

    glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );



    for (y=0; y<gliWindowHeight; y+=2)
    {
        glBegin(GL_LINES);
        glVertex3f(0, y, 0);
        glVertex3f(gliWindowWidth, y, 0);
        glEnd();
    }

/*
    for(int x=0;x<kContextWidth;x+=2) {
        glBegin(GL_LINES);
        glVertex3f(x, 0, 0);
        glVertex3f(x, kContextHeight, 0);
        glEnd();
    }
*/

    glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
}

// www.gali-3d.com/archive/articles/StereoOpenGL/StereoscopicOpenGLTutorial.php
void interlaced_glDrawBuffer (GLenum mode)
{
    real_glDrawBuffer (mode);

    if (QBState.current == GL_BACK_LEFT
    ||  QBState.current == GL_FRONT_LEFT
    ||  QBState.current == GL_LEFT)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 1); // draws if stencil <> 1
        glDisable (GL_STENCIL_TEST);
    } else
    if (QBState.current == GL_BACK_RIGHT
    ||  QBState.current == GL_FRONT_RIGHT
    ||  QBState.current == GL_RIGHT)
    {
        glStencilFunc (GL_EQUAL, 1, 1); // draws if stencil <> 0
        glEnable (GL_STENCIL_TEST);

    }
    else
    {   // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
        WARNING ("writing in (GL_FRONT, GL_BACK or GL_FRONT_AND_BACK)"
                 "not yet supported in interlaced");
    }
}

XVisualInfo *interlaced_glXChooseVisual
    (Display *dpy, int screen, int *attribList)
{
    XVisualInfo *r;
    int i = 0;

    while (attribList[i] != None)
    {
        if (attribList[i] == GLX_STENCIL_SIZE)
        {
            FATAL_ERROR ("glXChooseVisual(GLX_STENCIL_SIZE) incompatible "\
                         "with interlaced display mode !\n");
        }
        i++;
    }

    attribList[i] = GLX_STENCIL_SIZE;
    attribList[i+1] = 1;
    attribList[i+2] = None;

    r = real_glXChooseVisual (dpy, screen, attribList);

    return r;
}
