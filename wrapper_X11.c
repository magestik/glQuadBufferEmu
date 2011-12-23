#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

// Wrapper for X11 - /usr/include/X11/Xlib.h

void handleEvent(XEvent *e){
	char buffer[10];
	int r;
         
	switch(e->type){
		case ConfigureNotify:
			if(DEBUG) fprintf(stderr, "ConfigureNotify (%d, %d)\n", QuadBufferWidth, QuadBufferHeight);
			QuadBufferHeight = e->xconfigure.height;
			QuadBufferWidth = e->xconfigure.width;
		break;
		
		case KeyPress:
			if(DEBUG) fprintf(stderr, "Key pressed\n"); // TODO: check "Print Screen" and "Escape"
			r = XLookupString(&e->xkey, buffer, sizeof(buffer), NULL, NULL);
            if (buffer[0] == 27) {
				fprintf(stderr, "QuadBufferEmu : Exit\n");
				exit(1);
            }
		break;
	}
}

Window XCreateWindow (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
	
	// http://www.xfree86.org/current/XCreateWindow.3.html
	
	QuadBufferHeight = height;
	QuadBufferWidth = width;
	
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

/* http://www.xfree86.org/current/XNextEvent.3.html */

int XNextEvent(Display *display, XEvent *event_return) {
	if(DEBUG) fprintf(stderr, "XNextEvent(.)\n");
	
	int r = real_XNextEvent(display, event_return);
	handleEvent(event_return);
	return r;
}

int XPeekEvent(Display *display, XEvent *event_return) {
	if(DEBUG) fprintf(stderr, "XPeekEvent(.)\n");
	
	int r = real_XPeekEvent(display, event_return);
	handleEvent(event_return);
	return r;
}

int XWindowEvent(Display *display, Window w, long event_mask, XEvent *event_return) {
	if(DEBUG) fprintf(stderr, "XWindowEvent(.)\n");
	
	int r = real_XWindowEvent(display, w, event_mask, event_return);
	handleEvent(event_return);
	return r;
}
