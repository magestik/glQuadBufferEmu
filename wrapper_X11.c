#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

// Wrapper for X11 - /usr/include/X11/Xlib.h

Window XCreateWindow (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
	
	// http://www.xfree86.org/current/XCreateWindow.3.html
	
	fprintf(stderr, "XCreateWindow(.)\n");
	
	if(wrap_XCreateWindow == NULL || QuadBufferEnabled == GL_FALSE) {
		return real_XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
	} else {
		return wrap_XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
	}
}

int XDestroyWindow (Display *display, Window w) {
	
	// http://www.xfree86.org/current/XDestroyWindow.3.html
	
	fprintf(stderr, "XDestroyWindow(.)\n");

	if(wrap_XDestroyWindow == NULL || QuadBufferEnabled == GL_FALSE) {
		return real_XDestroyWindow(display, w);
	} else {
		return wrap_XDestroyWindow(display, w);
	}
}
