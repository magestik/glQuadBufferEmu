#ifndef H__FRAMESEQUENTIAL
#define H__FRAMESEQUENTIAL

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


typedef enum
{
    NO_GLASSES,
    NV3DVISION
} FRAMESEQUENTIAL_GLASSES;

typedef struct frameSequentialState
{
    GLenum buffer;

    /* the user have shutter glasses or not */
    FRAMESEQUENTIAL_GLASSES glasses;

    /* nvstusb context to use with NVIDIA 3D Vision */
    struct nvstusb_context *ctx;

} FRAMESEQUENTIAL_STATE;


void initFrameSequentialMode(void);
void wait_vblank(void);

/* Transformation functions */
/*
int glXSwapIntervalSGI (int interval);
int glXGetVideoSyncSGI (unsigned int *count);
int glXWaitVideoSyncSGI (int divisor, int remainder, unsigned int *count);
*/
void frameSequential_glDrawBuffer(GLenum mode);
void frameSequential_glXSwapBuffers(Display * dpy, GLXDrawable drawable);

#endif /* H__FRAMESEQUENTIAL */
