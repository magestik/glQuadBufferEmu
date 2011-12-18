#include "../glQuadBufferEmu.h"
#include "../wrappers.h"

#include "monoscopic.h"

void initMonoscopicMode(void){
	monoscopicBuffer = GL_BACK_LEFT;
	
	wrap_glDrawBuffer = monoscopic_glDrawBuffer;
}

void monoscopic_glDrawBuffer(GLenum mode) {
	if(QuadBufferCurrent == monoscopicBuffer){
		real_glDrawBuffer(mode);
	} else {
		real_glDrawBuffer(GL_NONE);
	}
}
