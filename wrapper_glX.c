#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>

#include "wrappers.h"

// Wrapper for glX - /usr/include/GL/glx.h

void calcFPS() {
	// TODO : calculer FPS
}

GLXFBConfig *glXChooseFBConfig(Display *dpy, int screen, const int *attrib_list, int *nelements) {
	int wrapped_attribList[60]; // 60 is enough ?
	int requested = 0;
	
	int i = 0;
	
	while(attrib_list[i] != None) {
		
		if(attrib_list[i] == GLX_STEREO) {
			
			if(attrib_list[i+1] == True) {
				requested += 2;
			} else if(attrib_list[i+1] != False) {
				requested++;
			} else { // attrib_list[i+1] == False
				//QuadBufferEnabled = GL_FALSE;
			}
			
		} else {
			wrapped_attribList[i-requested] = attrib_list[i];
		}

		i++;
	}

	wrapped_attribList[i-requested] = None;

	if(requested > 0) {
		QuadBufferEnabled = GL_TRUE;
		fprintf(stderr, "glXChooseFBConfig(GLX_STEREO)\n");
	} else {
		//QuadBufferEnabled = GL_FALSE;
		fprintf(stderr, "glXChooseFBConfig(.)\n");
	}
	
	if(wrap_glXChooseFBConfig == NULL || QuadBufferEnabled == GL_FALSE) {
		return real_glXChooseFBConfig(dpy, screen, (const int *)wrapped_attribList, nelements);
	} else {
		return wrap_glXChooseFBConfig(dpy, screen, (const int *)wrapped_attribList, nelements);
	}
}

XVisualInfo *glXChooseVisual(Display *dpy, int screen, int *attribList){	
	int wrapped_attribList[60]; // 60 is enough ?
	int requested = 0;
	
	int i = 0;
	
	while(attribList[i] != None) {
		
		if(attribList[i] == GLX_STEREO) {
			requested++;
		} else {
			wrapped_attribList[i-requested] = attribList[i];
		}

		i++;
	}

	wrapped_attribList[i-requested] = None;
	
	if(requested > 0) {
		QuadBufferEnabled = GL_TRUE;
		fprintf(stderr, "glXChooseVisual(GLX_STEREO)\n");
	} else {
		//QuadBufferEnabled = GL_FALSE;
		fprintf(stderr, "glXChooseVisual(.)\n");
	}

    
	if(wrap_glXChooseVisual == NULL || QuadBufferEnabled == GL_FALSE) {
		return real_glXChooseVisual(dpy, screen, wrapped_attribList);
	} else {
		return wrap_glXChooseVisual(dpy, screen, wrapped_attribList);
	}
}

int glXGetConfig(Display *dpy, XVisualInfo *vis, int attrib, int *value) {
	if(attrib == GLX_STEREO){
		*value = True;
		return 0;
	} else {
		return real_glXGetConfig(dpy, vis, attrib, value);
	}
}

int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value) {
	if(attribute == GLX_STEREO){
		if(QuadBufferEnabled == GL_TRUE) {
			*value = True;
		} else {
			*value = False;
		}
		return 0;
	} else {
		return real_glXGetFBConfigAttrib(dpy, config, attribute, value);
	}
}

void (*glXGetProcAddress(const GLubyte *procname))( void ) {
	if(DEBUG) fprintf(stderr, "glXGetProcAddress(%s)\n", procname);
	
	void *r = QuadBufferEmuFindFunction((const char *)procname);
	
	if( r != NULL ) {
		return r;
	} else {
		return real_glXGetProcAddress(procname);
	}
}

void (*glXGetProcAddressARB(const GLubyte *procname))( void ) {
	if(DEBUG) fprintf(stderr, "glXGetProcAddressARB(%s)\n", procname);

	void *r = QuadBufferEmuFindFunction((const char *)procname);
	
	if( r != NULL ) {
		return r;
	} else {
		return real_glXGetProcAddressARB(procname);
	}
}

void glXSwapBuffers(Display * dpy, GLXDrawable drawable){
	if(DEBUG) {
		fprintf(stderr, "glXSwapBuffers(.)\n");
		calcFPS();
	}
    
	if(wrap_glXSwapBuffers == NULL || QuadBufferEnabled == GL_FALSE) {
		real_glXSwapBuffers(dpy, drawable);
	} else {
		wrap_glXSwapBuffers(dpy, drawable);
	}
}
