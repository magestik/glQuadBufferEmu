#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../glQuadBufferEmu.h"
#include "../wrappers.h"
#include "frame-sequential.h"

void (*swapInterval)(int);

int (*glXGetVideoSyncSGI) (unsigned int *count);
int (*glXWaitVideoSyncSGI) (int divisor, int remainder, unsigned int *count);

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
        fprintf (stderr, "glXSwapInterval(1)\n");
    #endif

    if (is_glx_extension_supported ("GLX_MESA_swap_control"))
    {
        QBState.framesequential.swapMethod =
            (void (*)(int)) real_glXGetProcAddress
                                ((const GLubyte*) "glXSwapIntervalMESA");
    }
    else if (is_glx_extension_supported ("GLX_SGI_swap_control"))
    {
        QBState.framesequential.swapMethod =
            (void (*)(int)) real_glXGetProcAddress
                                ((const GLubyte*) "glXSwapIntervalSGI");
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

    if (getenv ("__GL_SYNC_TO_VBLANK")) {
            fprintf (stderr, "__GL_SYNC_TO_VBLANK defined\n");
    } else {
		glXWaitVideoSyncSGI = NULL;
		glXGetVideoSyncSGI = NULL;
		
		if (real_glXGetProcAddressARB) {
			glXWaitVideoSyncSGI = (void *) real_glXGetProcAddressARB((unsigned char *)"glXWaitVideoSyncSGI");
			glXGetVideoSyncSGI = (void *) real_glXGetProcAddressARB((unsigned char *)"glXGetVideoSyncSGI");
        }

        if (glXWaitVideoSyncSGI != NULL && glXGetVideoSyncSGI != NULL ) {
            fprintf (stderr, "glXWaitVideoSyncSGI && glXGetVideoSyncSGI\n");
        } else {
            frameSequential_glXSwapInterval(1);
        }
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

	unsigned int counter;
	if (glXWaitVideoSyncSGI != NULL && glXGetVideoSyncSGI != NULL ) {
		glXGetVideoSyncSGI(&counter);
		glXWaitVideoSyncSGI(2, (counter+1)%2, &counter);
	}
      
	
    if (QBState.framesequential.buffer == GL_LEFT) {
        QBState.framesequential.buffer = GL_RIGHT;
    } else {
        QBState.framesequential.buffer = GL_LEFT;
    }

    real_glXSwapBuffers (dpy, drawable);
}
