#include "../glQuadBufferEmu.h"
#include "side-by-side.h"

// Buffers: 6/9 (TODO: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)

void initSideBySideMode (void) {
    /* TODO : Hardware page-flipping for active shutter glasses
     *        ( X server viewport + GenLock ) */

    /*XF86VidModeModeInfo **modes;
    int modeNum, bestMode;*/
    
    // Change resolution : http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
    /*if(QBState.sidebyside.mode == SBS_FRAMEPACKED) { // Go to HDMI 1.4 resolution (1920x2205@24 or 1280x1470@60)
		XF86VidModeGetAllModeLines(display, scrnum, &modeNum, &modes);
		desktopMode = *modes[0];
		bestMode = 0;
		for (i = 0; i < modeNum; i++) {
			if ( modes[i]->hdisplay == 1920 && modes[i]->vdisplay == 2205 ) { // the best
				bestMode = i;
			}
			if ( modes[i]->hdisplay == 1280 && modes[i]->vdisplay == 1470 && bestMode == 0) {
				bestMode = i;
			}
		}

		XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
		XF86VidModeSetViewPort(display, screen, 0, 0);
		width = modes[bestMode]->hdisplay;
		height = modes[bestMode]->vdisplay;
		XFree(modes);
	}*/
	
    wrap_glDrawBuffer = sideBySide_glDrawBuffer;
    wrap_glGetIntegerv = sideBySide_glGetIntegerv;
    wrap_glScissor = sideBySide_glScissor;
    wrap_glViewport = sideBySide_glViewport;
    
    setCorrectViewport();
}

void setCorrectViewport (void) {
    switch (QBState.sidebyside.mode) {
		
		case SBS_LEFTRIGHT:
			setLeftRightViewport();
        break;

		case SBS_TOPBOTTOM:
			setTopBottomViewport ();
        break;

		case SBS_FRAMEPACKED:
			setFramePackedViewport();
        break;

		default:
			FATAL_ERROR ("Side-by-side mode not supported");
    }
}

void setTopBottomViewport (void) {
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = 0;
    QBState.sidebyside.rightViewport[1] = QBState.height / 2;

    QBState.sidebyside.ratio[0] = 1;
    QBState.sidebyside.ratio[1] = 2;
}

void setLeftRightViewport (void) {
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = QBState.width / 2;
    QBState.sidebyside.rightViewport[1] = 0;

    QBState.sidebyside.ratio[0] = 2;
    QBState.sidebyside.ratio[1] = 1;
}

void setFramePackedViewport (void) {
    QBState.sidebyside.leftViewport[0] = 0;
    QBState.sidebyside.leftViewport[1] = 0;

    QBState.sidebyside.rightViewport[0] = 0;
    /* MUST be in fullscreen and in the correct resolution for this to work correctly */
    QBState.sidebyside.rightViewport[1] = (QBState.height / 2) + (QBState.height / 49);

    QBState.sidebyside.ratio[0] = 1;
    QBState.sidebyside.ratio[1] = 2;

    /*
    real_glDrawBuffer(GL_BACK);
    if( glIsEnabled(GL_SCISSOR_TEST) ) glDisable(GL_SCISSOR_TEST);
    glScissor(0, QBState.height/2, QBState.width, (QBState.height/49));
    glClear(GL_COLOR_BUFFER_BIT);
    if( !glIsEnabled(GL_SCISSOR_TEST) ) glEnable(GL_SCISSOR_TEST);
    */
}

/* GL WRAP FUNCTIONS */

void sideBySide_glDrawBuffer (GLenum mode) {

    setCorrectViewport(); // in case height or width changed

    if(glIsEnabled (GL_SCISSOR_TEST)) real_glDisable (GL_SCISSOR_TEST);

    real_glDrawBuffer (mode);
    
	sideBySide_glViewport(0, 0, QBState.width, QBState.height);
	sideBySide_glScissor(0, 0, QBState.width, QBState.height);

    if (!glIsEnabled (GL_SCISSOR_TEST)) real_glEnable (GL_SCISSOR_TEST);
}

void sideBySide_glGetIntegerv (GLenum pname, GLint * params) {
	
	real_glGetIntegerv (pname, params);
	
	if (pname == GL_VIEWPORT || pname == GL_SCISSOR_BOX) {
		if (QBState.current == GL_BACK_LEFT ||  QBState.current == GL_FRONT_LEFT ||  QBState.current == GL_LEFT) {
			
			params[0] -= QBState.sidebyside.leftViewport[0];
			params[1] -= QBState.sidebyside.leftViewport[1];
			params[2] *= QBState.sidebyside.ratio[0];
			params[3] *= QBState.sidebyside.ratio[1];
			
		} else if (QBState.current == GL_BACK_RIGHT ||  QBState.current == GL_FRONT_RIGHT ||  QBState.current == GL_RIGHT) {
			
			params[0] -= QBState.sidebyside.rightViewport[0];
			params[1] -= QBState.sidebyside.rightViewport[1];
			params[2] *= QBState.sidebyside.ratio[0];
			params[3] *= QBState.sidebyside.ratio[1];
			
		} else {
			
			params[0] -= QBState.sidebyside.leftViewport[0];
			params[1] -= QBState.sidebyside.leftViewport[1];
			params[2] *= QBState.sidebyside.ratio[0];
			params[3] *= QBState.sidebyside.ratio[1];
			
		}
	}
}

void sideBySide_glScissor (GLint x, GLint y, GLsizei width, GLsizei height) {

    if (QBState.current == GL_BACK_LEFT ||  QBState.current == GL_FRONT_LEFT ||  QBState.current == GL_LEFT) {
        
        real_glScissor (QBState.sidebyside.leftViewport[0] + x,
                        QBState.sidebyside.leftViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
   
    } else if (QBState.current == GL_BACK_RIGHT ||  QBState.current == GL_FRONT_RIGHT ||  QBState.current == GL_RIGHT) {
        
        real_glScissor (QBState.sidebyside.rightViewport[0] + x,
                        QBState.sidebyside.rightViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
    
    } else {
        
        real_glScissor (QBState.sidebyside.leftViewport[0] + x,
                        QBState.sidebyside.leftViewport[1] + y,
                        width / QBState.sidebyside.ratio[0],
                        height / QBState.sidebyside.ratio[1]);
    }
}

void sideBySide_glViewport (GLint x, GLint y, GLsizei width, GLsizei height) {

    if (QBState.current == GL_BACK_LEFT ||  QBState.current == GL_FRONT_LEFT ||  QBState.current == GL_LEFT) {
        
        real_glViewport (QBState.sidebyside.leftViewport[0] + x,
                         QBState.sidebyside.leftViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
                         
    } else if (QBState.current == GL_BACK_RIGHT ||  QBState.current == GL_FRONT_RIGHT ||  QBState.current == GL_RIGHT) {
        
        real_glViewport (QBState.sidebyside.rightViewport[0] + x,
                         QBState.sidebyside.rightViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
                         
    } else {
        
        real_glViewport (QBState.sidebyside.leftViewport[0] + x,
                         QBState.sidebyside.leftViewport[1] + y,
                         width / QBState.sidebyside.ratio[0],
                         height / QBState.sidebyside.ratio[1]);
    }
}
