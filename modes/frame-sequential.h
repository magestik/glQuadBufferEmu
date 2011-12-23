#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <GL/glxext.h>
#include <GL/glext.h>

extern int glXSwapIntervalSGI (int interval);
extern int glXGetVideoSyncSGI (unsigned int *count);
extern int glXWaitVideoSyncSGI (int divisor, int remainder, unsigned int *count);

#define NV_NONE 0
#define NV_3D_VISION 1

int glasses;
GLenum frameSequentialBuffer;

void initFrameSequentialMode(void);
void wait_vblank(void);

/* Transformation functions */
void frameSequential_glDrawBuffer(GLenum mode);
void frameSequential_glXSwapBuffers(Display * dpy, GLXDrawable drawable);


