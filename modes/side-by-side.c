#include <stdio.h>
#include <stdlib.h>

#include "../glQuadBufferEmu.h"
#include "../wrappers.h"
#include "side-by-side.h"

// Buffers: 6/9 (TODO: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)

void initSideBySideMode (void)
{
    /* TODO : Hardware page-flipping for active shutter glasses
     *        ( X server viewport + GenLock ) */

    wrap_glDrawBuffer = sideBySide_glDrawBuffer;
    wrap_glGetIntegerv = sideBySide_glGetIntegerv;
    wrap_glViewport = sideBySide_glViewport;
    wrap_XCreateWindow = sideBySide_XCreateWindow;
}

void setCorrectViewport (void)
{
    switch (QBState.sidebyside.mode)
    {
    case SBS_SIDEBYSIDE:
        setLeftRightViewport();
        break;

    case SBS_TOPBOTTOM:
        setTopBottomViewport ();
        break;

    case SBS_FRAMEPACKED:
        setFramePackedViewport();
        break;

    default:
        FATAL_ERROR ("Side by side mode not supported");
    }
}

void setTopBottomViewport (void)
{
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = 0;
    QBState.sidebyside.rightViewport[1] = QBState.height / 2;

    QBState.sidebyside.ratio[0] = 1;
    QBState.sidebyside.ratio[1] = 2;
}

void setLeftRightViewport (void)
{
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = QBState.width / 2;
    QBState.sidebyside.rightViewport[1] = 0;

    QBState.sidebyside.ratio[0] = 2;
    QBState.sidebyside.ratio[1] = 1;
}

void setFramePackedViewport (void)
{
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = 0;
    /* MUST be in fullscreen and in the correct resolution
     * for this to work correctly */
    QBState.sidebyside.rightViewport[1]
                            = (QBState.height / 2) + (QBState.height / 49);

    QBState.sidebyside.ratio[0] = 1;
    QBState.sidebyside.ratio[1] = 2;

    /*
    real_glDrawBuffer(GL_BACK);
    if( glIsEnabled(GL_SCISSOR_TEST) ) glDisable(GL_SCISSOR_TEST);
    glScissor(0, QBState.height/2, QBState.width, (QBState.height/49));
    glClear(GL_COLOR_BUFFER_BIT);
    if( !glIsEnabled(GL_SCISSOR_TEST) ) glEnable(GL_SCISSOR_TEST);
    */
}

/* GL */

void sideBySide_glDrawBuffer (GLenum mode)
{

    setCorrectViewport (); // in case height or width changed

    if(glIsEnabled (GL_SCISSOR_TEST))
    {
        glDisable (GL_SCISSOR_TEST);
    }

    real_glDrawBuffer (mode);

    if (QBState.current == GL_BACK_LEFT
    ||  QBState.current == GL_FRONT_LEFT
    ||  QBState.current == GL_LEFT)
    {
        real_glViewport (QBState.sidebyside.leftViewport[0],
                         QBState.sidebyside.leftViewport[1],
                         QBState.width / QBState.sidebyside.ratio[0],
                         QBState.height / QBState.sidebyside.ratio[1]);

        real_glScissor (QBState.sidebyside.leftViewport[0],
                        QBState.sidebyside.leftViewport[1],
                        QBState.width / QBState.sidebyside.ratio[0],
                        QBState.height / QBState.sidebyside.ratio[1]);

    } else
    if (QBState.current == GL_BACK_RIGHT
    ||  QBState.current == GL_FRONT_RIGHT
    ||  QBState.current == GL_RIGHT)
    {
        real_glViewport (QBState.sidebyside.rightViewport[0],
                         QBState.sidebyside.rightViewport[1],
                         QBState.width / QBState.sidebyside.ratio[0],
                         QBState.height / QBState.sidebyside.ratio[1]);

        real_glScissor (QBState.sidebyside.rightViewport[0],
                        QBState.sidebyside.rightViewport[1],
                        QBState.width / QBState.sidebyside.ratio[0],
                        QBState.height / QBState.sidebyside.ratio[1]);

    }
    else
    { // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
        WARNING ("writing in (GL_FRONT, GL_BACK or GL_FRONT_AND_BACK)"
                 "not yet supported in side-by-side");
        real_glViewport
            (QBState.sidebyside.leftViewport[0],
             QBState.sidebyside.leftViewport[1],
             QBState.width / QBState.sidebyside.ratio[0],
             QBState.height / QBState.sidebyside.ratio[1]);
        real_glScissor
            (QBState.sidebyside.leftViewport[0],
             QBState.sidebyside.leftViewport[1],
             QBState.width / QBState.sidebyside.ratio[0],
             QBState.height / QBState.sidebyside.ratio[1]);
    }

    if (!glIsEnabled (GL_SCISSOR_TEST))
        glEnable (GL_SCISSOR_TEST);
}

void sideBySide_glGetIntegerv (GLenum pname, GLint * params)
{
    real_glGetIntegerv (pname, params);

    if (pname == GL_VIEWPORT) {
        if (QBState.current == GL_BACK_RIGHT
        ||  QBState.current == GL_FRONT_RIGHT
        ||  QBState.current == GL_RIGHT)
        {
            params[0] -= QBState.sidebyside.rightViewport[0];
            params[1] -= QBState.sidebyside.rightViewport[1];
            params[2] *= QBState.sidebyside.ratio[0];
            params[3] *= QBState.sidebyside.ratio[1];
        }
    }
}

void sideBySide_glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
    #ifdef DEBUG
        fprintf(stderr, "glScissor(%d, %d, %d, %d)\n", x, y, width, height);
    #endif

    if (QBState.current == GL_BACK_LEFT
    ||  QBState.current == GL_FRONT_LEFT
    ||  QBState.current == GL_LEFT)
    {
        real_glScissor (QBState.sidebyside.leftViewport[0] + x,
                        QBState.sidebyside.leftViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
    } else
    if (QBState.current == GL_BACK_RIGHT
    ||  QBState.current == GL_FRONT_RIGHT
    ||  QBState.current == GL_RIGHT)
    {
        real_glScissor (QBState.sidebyside.rightViewport[0] + x,
                        QBState.sidebyside.rightViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
    } else {
        real_glScissor (QBState.sidebyside.leftViewport[0] + x,
                        QBState.sidebyside.leftViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
    }
}

void sideBySide_glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
    #ifdef DEBUG
        fprintf(stderr, "glViewport(%d, %d, %d, %d)\n", x, y, width, height);
    #endif

    if (QBState.current == GL_BACK_LEFT
    ||  QBState.current == GL_FRONT_LEFT
    ||  QBState.current == GL_LEFT)
    {
        real_glViewport (QBState.sidebyside.leftViewport[0] + x,
                         QBState.sidebyside.leftViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
    }
    else
    if (QBState.current == GL_BACK_RIGHT
    ||  QBState.current == GL_FRONT_RIGHT
    ||  QBState.current == GL_RIGHT)
    {
        real_glViewport (QBState.sidebyside.rightViewport[0] + x,
                         QBState.sidebyside.rightViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
    } else {
        real_glViewport (QBState.sidebyside.leftViewport[0] + x,
                         QBState.sidebyside.leftViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
    }
}

/* X11 */
Window sideBySide_XCreateWindow
    (Display *display, Window parent, int x, int y, unsigned int width,
     unsigned int height, unsigned int border_width, int depth,
     unsigned int class, Visual *visual, unsigned long valuemask,
     XSetWindowAttributes *attributes)
{

    /*
    XF86VidModeModeInfo **modes;
    int modeNum, bestMode;
    */

    if (QBState.fullscreen == GL_TRUE)
    {
        int scrnum = DefaultScreen (display); // FIXME : Always DefaultScreen ?

        // Go to HDMI 1.4 resolution    (1920x2205@24 or 1280x1470@60)
        // http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
        /*
        XF86VidModeGetAllModeLines(display, scrnum, &modeNum, &modes);
        desktopMode = *modes[0];
        bestMode = 0;
        for (i = 0; i < modeNum; i++) {
            if ( modes[i]->hdisplay == 1920 && modes[i]->vdisplay == 2205 ) { // the best
                bestMode = i;
            }
            if ( modes[i]->hdisplay == 1280 && modes[i]->vdisplay == 1470 && bestMode == 0) {
                bestMode = i;
            }
        }

        XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
        XF86VidModeSetViewPort(display, screen, 0, 0);
        width = modes[bestMode]->hdisplay;
        height = modes[bestMode]->vdisplay;
        XFree(modes);
        */

        x = 0; y = 0;
        width = DisplayWidth (display, scrnum );
        height = DisplayHeight (display, scrnum );

        attributes->override_redirect = True;
        valuemask = valuemask | CWOverrideRedirect;
    }

    QBState.height = height;
    QBState.width = width;
    setCorrectViewport ();

    return
        real_XCreateWindow (display, parent, x, y, width, height, border_width,
                            depth, class, visual, valuemask, attributes);
}

int sideBySide_XDestroyWindow (Display *display, Window w)
{
    // Get the resolution in default mode
    /*
    XF86VidModeSwitchToMode(display, screen, &desktopMode);
    XF86VidModeSetViewPort(display, screen, 0, 0);
    */

    return real_XDestroyWindow (display, w);
}
