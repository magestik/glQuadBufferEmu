#include "glQuadBufferEmu.h"
#include "frame-sequential.h"

#include "nvstusb.h"

#include <unistd.h>
#include <fcntl.h>

struct nvstusb_context *ctx = 0;
int vblank_method = 0;

int (*glXGetVideoSyncSGI) (unsigned int *count);
int (*glXWaitVideoSyncSGI) (int divisor, int remainder, unsigned int *count);

//int proc_glasses = 0;

// Buffers: 9/9

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

int force_sync() {
	void (*swapInterval) (int);

    if (is_glx_extension_supported ("GLX_MESA_swap_control")) {
        swapInterval = (void (*)(int)) real_glXGetProcAddress((const GLubyte *) "glXSwapIntervalMESA");
    } else if (is_glx_extension_supported ("GLX_SGI_swap_control")) {
        swapInterval = (void (*)(int)) real_glXGetProcAddress((const GLubyte *) "glXSwapIntervalSGI");
    } else {
        return 0;
    }

    swapInterval(1);
    return 1;
}


void initFrameSequentialMode (void) {
    QBState.framesequential.buffer = GL_LEFT;

	// if(nvidia 3d vision) {
		ctx = nvstusb_init(NULL);
		if (ctx == 0) {
			fprintf(stderr, "could not initialize NVIDIA 3D Stereo Controller\n");
			exit(EXIT_FAILURE);
		}

		nvstusb_set_rate(ctx, 120.0);
	// }

	/* Vblank init */
	/* NVIDIA VBlank syncing environment variable defined, signal it and disable
	 * any attempt to application side method */

	if ( getenv("__GL_SYNC_TO_VBLANK") ) {

		fprintf (stderr, "__GL_SYNC_TO_VBLANK defined\n");
		vblank_method = 1;

	} else {

		/* Sync Video */
		glXGetVideoSyncSGI = NULL;
        glXWaitVideoSyncSGI = NULL;

		glXGetVideoSyncSGI = (PFNGLXGETVIDEOSYNCSGIPROC)real_glXGetProcAddress((unsigned char *)"glXGetVideoSyncSGI");
		glXWaitVideoSyncSGI = (PFNGLXWAITVIDEOSYNCSGIPROC)real_glXGetProcAddress((unsigned char *)"glXWaitVideoSyncSGI");

		 if (glXWaitVideoSyncSGI != NULL && glXGetVideoSyncSGI != NULL ) {
			vblank_method = 2;
			fprintf(stderr, "glXWaitVideoSyncSGI && glXGetVideoSyncSGI supported\n");
		} else {
			vblank_method = force_sync();
		}
	}

	if (vblank_method == 0) {
		fprintf(stderr, "glQuadBufferEmu won't sync anything !\n");
	}

	/*
	proc_glasses = open("/proc/sys/glasses3d/eye", O_WRONLY);
	if(proc_glasses <= 0){
		printf("No glasses supported, you are on your own !");
	}
	*/
    wrap_glDrawBuffer = frameSequential_glDrawBuffer;
    wrap_glXSwapBuffers = frameSequential_glXSwapBuffers;
}

void frameSequential_glDrawBuffer (GLenum mode) {

	switch(QBState.current) {
		case GL_FRONT_LEFT:
		case GL_BACK_LEFT:
		case GL_LEFT:

			if (QBState.framesequential.buffer == GL_LEFT) {
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}

        break;

        case GL_FRONT_RIGHT:
        case GL_BACK_RIGHT:
        case GL_RIGHT:

			if (QBState.framesequential.buffer == GL_RIGHT) {
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}

		break;

		default: // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
			real_glDrawBuffer(mode);
	}
}

void __swap(){}

void frameSequential_glXSwapBuffers (Display * dpy, GLXDrawable drawable) {

    unsigned int counter = 0;
    int eye;

    if (QBState.framesequential.buffer == GL_LEFT) {
		QBState.framesequential.buffer = GL_RIGHT;
		eye = 0;
	} else {
		QBState.framesequential.buffer = GL_LEFT;
		eye = 1;
	}

	switch(vblank_method) {
		case 1: // auto-synced
			real_glXSwapBuffers (dpy, drawable);
			nvstusb_set_eye(ctx, eye);
		break;

		case 2:
			glXGetVideoSyncSGI (&counter);
			glXWaitVideoSyncSGI (2, (counter + 1) % 2, &counter);
			//glXWaitVideoSyncSGI (2, ((QBState.framesequential.buffer == GL_LEFT) ? 1 : 0), &counter); // on attend que (counter % 2) = side

			nvstusb_set_eye(ctx, eye);

			real_glXSwapBuffers (dpy, drawable);
		break;
	}

    //glXWaitGL();


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
