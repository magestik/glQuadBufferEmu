#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "monoscopic.h"

// Buffers: 9/9

void initMonoscopicMode(void){
	monoscopicBuffer = GL_LEFT;
	
	wrap_glDrawBuffer = monoscopic_glDrawBuffer;
}

void monoscopic_glDrawBuffer(GLenum mode) {
	switch(QuadBufferCurrent){
		case GL_FRONT_LEFT:
		case GL_BACK_LEFT:
		case GL_LEFT:
			if(monoscopicBuffer == GL_LEFT){
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}	
		break;
		
		case GL_FRONT_RIGHT:
		case GL_BACK_RIGHT:
		case GL_RIGHT:
			if(monoscopicBuffer == GL_RIGHT){
				real_glDrawBuffer(mode);
			} else {
				real_glDrawBuffer(GL_NONE);
			}	
		break;

		default: // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
			real_glDrawBuffer(mode);
	}
}
