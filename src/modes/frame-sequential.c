#include "glQuadBufferEmu.h"
#include "frame-sequential.h"

#include "nvstusb.h"

#include <unistd.h>
#include <fcntl.h>

struct nvstusb_context *ctx = 0;

/*
void (*swapInterval) (int);

int (*glXGetVideoSyncSGI) (unsigned int *count);
int (*glXWaitVideoSyncSGI) (int divisor, int remainder, unsigned int *count);

int proc_glasses = 0;
*/
// Buffers: 9/9

/*
 * TODO:
 * - Sync with nouveau
 * - Detections improvement
*/

/*
00226         glXGetSyncValuesOML = (void *)glXGetProcAddress((unsigned char *)"glXGetSyncValuesOML");
00227         glXGetMscRateOML = (void *)glXGetProcAddress((unsigned char *)"glXGetMscRateOML");
00228         glXSwapBuffersMscOML = (void *)glXGetProcAddress((unsigned char *)"glXSwapBuffersMscOML");
00229         glXWaitForMscOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForMscOML");
00230         glXWaitForSbcOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForSbcOML");
*/
/*
static int is_glx_extension_supported (const char *query) {
    Display *dpy = XOpenDisplay (NULL);
    const char *glx_extensions = NULL;
    int ret = 1;
    int scrnum;

    if (!dpy) {
        WARNING ("couldn't open display");
        ret = -1;
    } else {
        scrnum = DefaultScreen (dpy);
        glx_extensions = glXQueryExtensionsString (dpy, scrnum);
        if (strstr (glx_extensions, query) == '\0')
            ret = 0;
    }

    #ifdef DEBUG
    if (ret == 1)
        fprintf (stderr, "is_glx_extension_supported (%s)\n"
                         "> extension is supported\n", query);
    else
        fprintf (stderr, "is_glx_extension_supported (%s)\n"
                         "[WW] extension is not supported\n", query);
    #endif

    return ret;
}


void frameSequential_glXSwapInterval (int i) {
    #ifdef DEBUG
        fprintf (stderr, "glXSwapInterval(1)\n");
    #endif

    if (is_glx_extension_supported ("GLX_MESA_swap_control")) {
        swapInterval = (void (*)(int)) real_glXGetProcAddress ((const GLubyte*) "glXSwapIntervalMESA");
    } else if (is_glx_extension_supported ("GLX_SGI_swap_control")) {
        swapInterval = (void (*)(int)) real_glXGetProcAddress ((const GLubyte*) "glXSwapIntervalSGI");
    } else {
        WARNING ("No sync method !!!");
    }

    swapInterval(i);
}
*/
void initFrameSequentialMode (void) {
    QBState.framesequential.buffer = GL_LEFT;

	ctx = nvstusb_init(NULL);
    if (0 == ctx) {
      fprintf(stderr, "could not initialize NVIDIA 3D Stereo Controller\n");
    }

	nvstusb_set_rate(ctx, 120.0);
/*
	proc_glasses = open("/proc/sys/glasses3d/eye", O_WRONLY);
	if(proc_glasses <= 0){
		printf("No glasses supported, you are on your own !");
	}

    if (getenv ("__GL_SYNC_TO_VBLANK")) {
        fprintf (stderr, "__GL_SYNC_TO_VBLANK defined\n");
    } else {
        glXWaitVideoSyncSGI = NULL;
        glXGetVideoSyncSGI = NULL;

        if (real_glXGetProcAddressARB) {
            glXWaitVideoSyncSGI =
                (void *) real_glXGetProcAddressARB
                                ((unsigned char *)"glXWaitVideoSyncSGI");
            glXGetVideoSyncSGI =
                (void *) real_glXGetProcAddressARB
                                ((unsigned char *)"glXGetVideoSyncSGI");
        }

        if (glXWaitVideoSyncSGI != NULL && glXGetVideoSyncSGI != NULL ) {
            fprintf (stderr, "glXWaitVideoSyncSGI && glXGetVideoSyncSGI\n");
        } else {
            frameSequential_glXSwapInterval(1);
        }
    }
*/
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

void __swap(){}

void frameSequential_glXSwapBuffers (Display * dpy, GLXDrawable drawable) {

    //unsigned int counter = 0;
/*
    if (glXWaitVideoSyncSGI != NULL && glXGetVideoSyncSGI != NULL)
    {
        //glXGetVideoSyncSGI (&counter);
        //glXWaitVideoSyncSGI (2, (counter + 1) % 2, &counter);
        glXWaitVideoSyncSGI (2, ((QBState.framesequential.buffer == GL_LEFT) ? 1 : 0), &counter); // on attend que (counter % 2) = side
    }
*/

    if (QBState.framesequential.buffer == GL_LEFT)
    {
        QBState.framesequential.buffer = GL_RIGHT;
         nvstusb_swap(ctx, 0, __swap);
    }
    else
    {
        QBState.framesequential.buffer = GL_LEFT;
         nvstusb_swap(ctx, 1, __swap);
    }

    //glXWaitGL();


    real_glXSwapBuffers (dpy, drawable);

/*
	if(proc_glasses > 0) {
		if(QBState.framesequential.buffer == GL_LEFT) {
			write(proc_glasses, "0", sizeof(char));
		} else {
			write(proc_glasses, "1", sizeof(char));
		}
	}
*/
}