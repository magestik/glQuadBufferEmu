#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "anaglyph.h"

void initAnaglyphMode(void){
	//anaglyphColors = REDCYAN;
	
	wrap_glDrawBuffer = anaglyph_glDrawBuffer;
}

void anaglyph_glDrawBuffer(GLenum mode) {
	// http://paulbourke.net/texture_colour/anaglyph/
	
	real_glDrawBuffer(mode);
	
	if(QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT || QuadBufferCurrent == GL_LEFT){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE); // on laisse passer que le rouge
	} else {
		glClear(GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE); // on laisse passer que le cyan
	}
}
