#ifndef H__WRAPPER_GL
#define H__WRAPPER_GL

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glQuadBufferEmu.h"

void glClear(GLbitfield mask);
void glDrawBuffer(GLenum mode);
void glDisable(GLenum cap);
void glEnable(GLenum cap);
void glGetBooleanv(GLenum pname, GLboolean * params);
void glGetDoublev(GLenum pname, GLdouble * params);
void glGetFloatv(GLenum pname, GLfloat * params);
void glGetIntegerv(GLenum pname, GLint * params);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

/* Real functions */
void (*real_glClear) (GLbitfield  mask);
void (*real_glDrawBuffer) (GLenum mode);
void (*real_glDisable) (GLenum cap);
void (*real_glEnable) (GLenum cap);
void (*real_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*real_glGetDoublev) (GLenum pname, GLdouble * params);
void (*real_glGetFloatv) (GLenum pname, GLfloat * params);
void (*real_glGetIntegerv) (GLenum pname, GLint * params);
void (*real_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*real_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

/* link to transform functions */
void (*wrap_glClear) (GLbitfield  mask);
void (*wrap_glDrawBuffer) (GLenum mode);
void (*wrap_glDisable) (GLenum  cap);
void (*wrap_glEnable) (GLenum  cap);
void (*wrap_glGetBooleanv) (GLenum pname, GLboolean * params);
void (*wrap_glGetDoublev) (GLenum pname, GLdouble * params);
void (*wrap_glGetFloatv) (GLenum pname, GLfloat * params);
void (*wrap_glGetIntegerv) (GLenum pname, GLint * params);
void (*wrap_glScissor) (GLint x, GLint  y, GLsizei  width, GLsizei height);
void (*wrap_glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

#endif /* H__WRAPPER_GL */
