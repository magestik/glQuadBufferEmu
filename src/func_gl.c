#include "glQuadBufferEmu.h"

/** Implementation of GL functions *********************************************/

/* http://www.opengl.org/sdk/docs/man/xhtml/glClear.xml */
void glClear (GLbitfield mask)
{
    #ifdef DEBUG
        fprintf(stderr, "glClear(0x%4X)\n", mask);
    #endif

    if (wrap_glClear == NULL || QBState.enabled == GL_FALSE) {
        real_glClear(mask);
    }  else {
        wrap_glClear(mask);
    }
}


/* http://www.opengl.org/sdk/docs/man/xhtml/glDrawBuffer.xml */
void glDrawBuffer (GLenum mode)
{
    const char *bname;
    GLboolean wrap = GL_TRUE;
    QBState.current = mode;
    (void) bname; /* to avoid boring "not used" warning */

    switch (mode)
    {
        // GL_ FRONT
        case GL_FRONT:
            bname = "GL_FRONT";
            mode = GL_FRONT;
        break;

        case GL_FRONT_LEFT:
            bname = "GL_FRONT_LEFT";
            mode = GL_FRONT;
        break;

        case GL_FRONT_RIGHT:
            bname = "GL_FRONT_RIGHT";
            mode = GL_FRONT;
        break;


        // GL_BACK
        case GL_BACK:
            bname = "GL_BACK";
            mode = GL_BACK;
        break;

        case GL_BACK_LEFT:
            bname = "GL_BACK_LEFT";
            mode = GL_BACK;
        break;

        case GL_BACK_RIGHT:
            bname = "GL_BACK_RIGHT";
            mode = GL_BACK;
        break;

        // GL_FRONT_AND_BACK
        case GL_FRONT_AND_BACK: // ALL 4 BUFFERS
            bname = "GL_FRONT_AND_BACK";
            mode = GL_FRONT_AND_BACK;
        break;


        // GL_LEFT  = GL_FRONT_LEFT + GL_BACK_LEFT
        case GL_LEFT:
            bname = "GL_LEFT";
            mode = GL_FRONT_AND_BACK;
        break;


        // GL_RIGHT = GL_FRONT_RIGHT + GL_BACK_RIGHT
        case GL_RIGHT:
            bname = "GL_RIGHT";
            mode = GL_FRONT_AND_BACK;
        break;

        case GL_NONE:
            bname = "GL_NONE";
            wrap = GL_FALSE;
        break;

        default: // GL_AUXi
            bname = "GL_AUXi"; // we don't need to know i value
            wrap = GL_FALSE;
    }

    #ifdef DEBUG
        fprintf (stderr, "glDrawBuffer(%s)\n", bname);
    #endif

    if (wrap_glDrawBuffer == NULL ||  QBState.enabled == GL_FALSE ||  wrap == GL_FALSE) {
        real_glDrawBuffer (mode);
    } else {
        wrap_glDrawBuffer (mode);
    }
}


/* http://www.opengl.org/sdk/docs/man/xhtml/glEnable.xml */
void glEnable (GLenum cap)
{
    #ifdef DEBUG
        fprintf (stderr, "glEnable(%d)\n", cap);
    #endif

    if (wrap_glEnable == NULL || QBState.enabled == GL_FALSE) {
        real_glEnable (cap);
    } else {
        wrap_glEnable (cap);
    }
}


void glDisable (GLenum cap)
{
    #ifdef DEBUG
        fprintf (stderr, "glDisable(%d)\n", cap);
    #endif

    if (wrap_glDisable == NULL || QBState.enabled == GL_FALSE) {
        real_glDisable (cap);
    } else {
        wrap_glDisable (cap);
    }
}


/* http://www.opengl.org/sdk/docs/man/xhtml/glGet.xml */
void glGetBooleanv (GLenum pname, GLboolean *params)
{
    #ifdef DEBUG
        fprintf (stderr, "glGetBooleanv(%d, %p)\n", pname, params);
    #endif

    switch (pname)
    {
    case GL_STEREO:
        *params = GL_TRUE;
        break;

    default:
        if (wrap_glGetBooleanv == NULL || QBState.enabled == GL_FALSE) {
            real_glGetBooleanv (pname, params);
        } else {
            wrap_glGetBooleanv (pname, params);
        }
    }
}


void glGetDoublev (GLenum pname, GLdouble * params)
{
    #ifdef DEBUG
        fprintf (stderr, "glGetDoublev(%d, %p)\n", pname, params);
    #endif

    if (wrap_glGetDoublev == NULL || QBState.enabled == GL_FALSE) {
        real_glGetDoublev (pname, params);
    } else {
        wrap_glGetDoublev (pname, params);
    }
}


void glGetFloatv (GLenum pname, GLfloat *params)
{
    #ifdef DEBUG
        fprintf (stderr, "glGetFloatv(%d, %p)\n", pname, params);
    #endif

    if (wrap_glGetFloatv == NULL || QBState.enabled == GL_FALSE) {
        real_glGetFloatv (pname, params);
    } else {
        wrap_glGetFloatv (pname, params);
    }
}


void glGetIntegerv (GLenum pname, GLint *params)
{
    #ifdef DEBUG
        fprintf (stderr, "glGetIntegerv(%d, %p)\n", pname, params);
    #endif

    switch (pname) {
		case GL_DRAW_BUFFER:
			*params = QBState.current;
        break;

		default:
			if (wrap_glGetIntegerv == NULL || QBState.enabled == GL_FALSE) {
				real_glGetIntegerv (pname, params);
			} else {
				wrap_glGetIntegerv (pname, params);
			}
	}
}


// http://www.opengl.org/sdk/docs/man/xhtml/glScissor.xml
void glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
    #ifdef DEBUG
        fprintf (stderr, "glScissor(%d, %d, %d, %d)\n", x, y, width, height);
    #endif

    if (wrap_glScissor == NULL || QBState.enabled == GL_FALSE) {
        real_glScissor (x, y, width, height);
    } else {
        wrap_glScissor (x, y, width, height);
    }
}


// http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
void glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
    #ifdef DEBUG
        fprintf (stderr, "glViewport(%d, %d, %d, %d)\n", x, y, width, height);
    #endif

    if(wrap_glViewport == NULL || QBState.enabled == GL_FALSE) {
        real_glViewport (x, y, width, height);
    } else {
        wrap_glViewport (x, y, width, height);
    }
}

/******************************************************************************/
