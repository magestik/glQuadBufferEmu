#ifndef H__MONOSCOPIC
#define H__MONOSCOPIC


#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


typedef struct monoscopicState
{
    GLenum     buffer;
    GLenum     view;
} MONOSCOPIC_STATE;


void initMonoscopicMode(void);

extern GLenum monoscopicBuffer;

/* Transformation functions */
void monoscopic_glDrawBuffer(GLenum mode);

#endif /* H__MONOSCOPIC */
