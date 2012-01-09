#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "frame-sequential.h"

// Buffers: 9/9

struct nvstusb_context *ctx = 0;

/*
00226         glXGetSyncValuesOML = (void *)glXGetProcAddress((unsigned char *)"glXGetSyncValuesOML");
00227         glXGetMscRateOML = (void *)glXGetProcAddress((unsigned char *)"glXGetMscRateOML");
00228         glXSwapBuffersMscOML = (void *)glXGetProcAddress((unsigned char *)"glXSwapBuffersMscOML");
00229         glXWaitForMscOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForMscOML");
00230         glXWaitForSbcOML = (void *)glXGetProcAddress((unsigned char *)"glXWaitForSbcOML");
00231         glXSwapInterval = (void *)glXGetProcAddress((unsigned char *)"glXSwapIntervalMESA");
*/

void initFrameSequentialMode(void){
	frameSequentialBuffer = GL_LEFT;
	
	if (getenv ("__GL_SYNC_TO_VBLANK")) {
		fprintf (stderr, "__GL_SYNC_TO_VBLANK defined\n");
	} else {
		fprintf (stderr, "No sync method !!!\n");
	}
  
	wrap_glDrawBuffer = frameSequential_glDrawBuffer;
	wrap_glXSwapBuffers = frameSequential_glXSwapBuffers;
}

void frameSequential_glDrawBuffer(GLenum mode) {
	switch(QuadBufferCurrent){
		case GL_FRONT_LEFT:
		case GL_BACK_LEFT:
		case GL_LEFT:
			if(frameSequentialBuffer == GL_LEFT){
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}	
		break;
		
		case GL_FRONT_RIGHT:
		case GL_BACK_RIGHT:
		case GL_RIGHT:
			if(frameSequentialBuffer == GL_RIGHT){
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}	
		break;

		default: // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
			real_glDrawBuffer(mode);
	}
}

void frameSequential_glXSwapBuffers(Display * dpy, GLXDrawable drawable){
	if(frameSequentialBuffer == GL_LEFT){
		frameSequentialBuffer = GL_RIGHT;
	} else {
		frameSequentialBuffer = GL_LEFT;
	}
	
	real_glXSwapBuffers(dpy, drawable);
}
