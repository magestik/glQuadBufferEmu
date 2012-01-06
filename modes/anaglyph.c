#include <stdio.h>
#include <stdlib.h>

#include "../wrappers.h"
#include "anaglyph.h"

// Buffers: 9/9

// http://paulbourke.net/texture_colour/anaglyph/

void initAnaglyphMode(void){	
	anaglyph_getColorMask(RED, anaglyphLeft);
	anaglyph_getColorMask(CYAN, anaglyphRight);
	
	wrap_glDrawBuffer = anaglyph_glDrawBuffer;
}

void anaglyph_getColorMask(int c, GLboolean *tab){
	switch(c){
		case WHITE:
			tab[0] = GL_FALSE;
			tab[1] = GL_FALSE;
			tab[2] = GL_FALSE;
		break;
		
		case BLUE:
			tab[0] = GL_FALSE;
			tab[1] = GL_FALSE;
			tab[2] = GL_TRUE;
		break;
			
		case GREEN:
			tab[0] = GL_FALSE;
			tab[1] = GL_TRUE;
			tab[2] = GL_FALSE;
		break;
			
		case RED:
			tab[0] = GL_FALSE;
			tab[1] = GL_TRUE;
			tab[2] = GL_TRUE;
		break;
			
		case CYAN:
			tab[0] = GL_TRUE;
			tab[1] = GL_FALSE;
			tab[2] = GL_FALSE;
		break;
			
		case MAGENTA:
			tab[0] = GL_TRUE;
			tab[1] = GL_FALSE;
			tab[2] = GL_TRUE;
		break;
			
		case YELLOW:
			tab[0] = GL_TRUE;
			tab[1] = GL_TRUE;
			tab[2] = GL_FALSE;
		break;
		
		default:
			tab[0] = GL_TRUE;
			tab[1] = GL_TRUE;
			tab[2] = GL_TRUE;
	}
}

void anaglyph_glDrawBuffer(GLenum mode) {	
	real_glDrawBuffer(mode);
	
	if(QuadBufferCurrent == GL_BACK_LEFT || QuadBufferCurrent == GL_FRONT_LEFT || QuadBufferCurrent == GL_LEFT){
		real_glClear(GL_DEPTH_BUFFER_BIT); // | GL_COLOR_BUFFER_BIT
		glColorMask(anaglyphLeft[0], anaglyphLeft[1], anaglyphLeft[2], GL_TRUE);
	
	} else if(QuadBufferCurrent == GL_BACK_RIGHT || QuadBufferCurrent == GL_FRONT_RIGHT || QuadBufferCurrent == GL_RIGHT){
		real_glClear(GL_DEPTH_BUFFER_BIT);
		glColorMask(anaglyphRight[0], anaglyphRight[1], anaglyphRight[2], GL_TRUE);
	
	} else { // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}
