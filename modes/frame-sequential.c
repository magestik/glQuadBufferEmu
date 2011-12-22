#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "frame-sequential.h"

struct nvstusb_context *ctx = 0;

void initFrameSequentialMode(void){
	frameSequentialBuffer = GL_BACK_LEFT;

	wrap_glDrawBuffer = frameSequential_glDrawBuffer;
	wrap_glXSwapBuffers = frameSequential_glXSwapBuffers;
}


void frameSequential_glDrawBuffer(GLenum mode) {
	if(QuadBufferCurrent == frameSequentialBuffer){
		real_glDrawBuffer(mode);
	} else {
		real_glDrawBuffer(GL_NONE);
	}
}


void frameSequential_glXSwapBuffers(Display * dpy, GLXDrawable drawable){
	
	if(frameSequentialBuffer == GL_BACK_LEFT){
		frameSequentialBuffer = GL_BACK_RIGHT;
	} else {
		frameSequentialBuffer = GL_BACK_LEFT;
	}
	
	real_glXSwapBuffers(dpy, drawable);
}
