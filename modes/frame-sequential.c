#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../glQuadBufferEmu.h"
#include "../wrappers.h"
#include "frame-sequential.h"


void (*swapInterval)(int);

// Buffers: 9/9


/*
00226         glXGetSyncValuesOML = (void *)glXGetProcAddress((unsigned char *)"glXGetSyncValuesOML");
00227         glXGetMscRateOML = (void *)glXGetProcAddress((unsigned char *)"glXGetMscRateOML");
00228         glXSwapBuffersMscOML = (void *)glXGetProcAddress((unsigned char *)"glXSwapBuffersMscOML");
00229         glXWaitForMscOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForMscOML");
00230         glXWaitForSbcOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForSbcOML");
*/



static int is_glx_extension_supported (const char *query)
{
    Display *dpy = XOpenDisplay (NULL);
    const char *glx_extensions = NULL;
    int ret = 1;
    int scrnum;

    if (!dpy)
    {
        WARNING ("couldn't open display");
        ret = -1;
    }
    else
    {
        scrnum = DefaultScreen (dpy);
        glx_extensions = glXQueryExtensionsString (dpy, scrnum);
        if (strstr (glx_extensions, query) == '\0')
            ret = 0;
    }

    return ret;
}


void frameSequential_glXSwapInterval (int i)
{
    #ifdef DEBUG
        fprintf (stderr, "frameSequentialSetSwapMethod()\n");
    #endif

    if (is_glx_extension_supported ("GLX_MESA_swap_control"))
    {
        QBState.framesequential.swapMethod =
            (void (*)(int)) real_glXGetProcAddress
                                ((const GLubyte*) "glXSwapIntervalMESA");
        #ifdef DEBUG
            fprintf (stderr, "glXSwapIntervalMESA(1)\n");
        #endif
    }
    else if (is_glx_extension_supported ("GLX_SGI_swap_control"))
    {
        QBState.framesequential.swapMethod =
            (void (*)(int)) real_glXGetProcAddress
                                ((const GLubyte*) "glXSwapIntervalSGI");
        #ifdef DEBUG
            fprintf (stderr, "glXSwapIntervalSGI(1)\n");
        #endif
    }
    else
    {
        WARNING ("No sync method !!!");
    }
    QBState.framesequential.swapMethod (i);
}

void initFrameSequentialMode(void)
{
    QBState.framesequential.buffer = GL_LEFT;

    if (getenv ("__GL_SYNC_TO_VBLANK"))
    {
            fprintf (stderr, "__GL_SYNC_TO_VBLANK defined\n");
    }
    else
    {
      /*  if (real_glXGetProcAddressARB) {
            glXWaitVideoSyncSGI = (void *)glXGetProcAddressARB((unsigned char *)"glXWaitVideoSyncSGI");
            glXGetVideoSyncSGI = (void *)glXGetProcAddressARB((unsigned char *)"glXGetVideoSyncSGI");
        }

        if (glXWaitVideoSyncSGI && glXGetVideoSyncSGI) {
            fprintf (stderr, "glXWaitVideoSyncSGI && glXGetVideoSyncSGI\n");
        } else { */
            frameSequential_glXSwapInterval (1);
        /*}*/
    }

    wrap_glDrawBuffer = frameSequential_glDrawBuffer;
    wrap_glXSwapBuffers = frameSequential_glXSwapBuffers;
}

void frameSequential_glDrawBuffer (GLenum mode)
{
    switch(QBState.current)
    {
    case GL_FRONT_LEFT:
    case GL_BACK_LEFT:
    case GL_LEFT:
        if (QBState.framesequential.buffer == GL_LEFT)
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
        if (QBState.framesequential.buffer == GL_RIGHT)
        {
            real_glDrawBuffer (mode);
        }
        else
        {
            real_glDrawBuffer (GL_NONE);
        }
        break;

    default: // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
        real_glDrawBuffer (mode);
    }
}

void frameSequential_glXSwapBuffers (Display * dpy, GLXDrawable drawable)
{
    if (QBState.framesequential.buffer == GL_LEFT)
    {
        QBState.framesequential.buffer = GL_RIGHT;
    }
    else
    {
        QBState.framesequential.buffer = GL_LEFT;
    }

    real_glXSwapBuffers (dpy, drawable);
}
