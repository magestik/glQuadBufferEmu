#include <stdio.h>
#include <stdlib.h>

#include "wrapper_X11.h"

// Wrapper for X11 - /usr/include/X11/Xlib.h

void handleEvent(XEvent *e){
    char buffer[10];

    switch(e->type){
        case ConfigureNotify:
            #ifdef DEBUG
                fprintf (stderr, "ConfigureNotify (%d, %d)\n",
                         QuadBufferWidth, QuadBufferHeight);
            #endif

            QuadBufferHeight = e->xconfigure.height;
            QuadBufferWidth = e->xconfigure.width;
        break;

        case KeyPress:
            #ifdef DEBUG
                /* TODO: check "Print Screen" and "Escape" */
                fprintf (stderr, "Key pressed\n");
            #endif

            XLookupString(&e->xkey, buffer, sizeof(buffer), NULL, NULL);
            if (buffer[0] == 27) {
                //fprintf(stderr, "QuadBufferEmu : Exit\n");
                //exit(1); // outch !
            }
        break;

        /*default:
             fprintf(stderr, "Unknown event\n");*/
    }
}

Window XCreateWindow (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
    #ifdef DEBUG
        fprintf(stderr, "XCreateWindow(.)\n");
    #endif
    Window ret;

    // http://www.xfree86.org/current/XCreateWindow.3.html

    QuadBufferHeight = height;
    QuadBufferWidth = width;

    if(wrap_XCreateWindow == NULL || QuadBufferEnabled == GL_FALSE) {
        ret = real_XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
    } else {
        ret = wrap_XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
    }

    return ret;
}

int XDestroyWindow (Display *display, Window w) {
    #ifdef DEBUG
        fprintf(stderr, "XDestroyWindow(.)\n");
    #endif
    int ret;

    // http://www.xfree86.org/current/XDestroyWindow.3.html

    if(wrap_XDestroyWindow == NULL || QuadBufferEnabled == GL_FALSE) {
        ret = real_XDestroyWindow(display, w);
    } else {
        ret = wrap_XDestroyWindow(display, w);
    }

    return ret;
}

/* http://www.xfree86.org/current/XNextEvent.3.html */

int XNextEvent(Display *display, XEvent *event_return) {
    #ifdef DEBUG
        fprintf(stderr, "XNextEvent(.)\n");
    #endif

    int r = real_XNextEvent(display, event_return);
    handleEvent(event_return);
    return r;
}

int XPeekEvent(Display *display, XEvent *event_return) {
    #ifdef DEBUG
        fprintf(stderr, "XPeekEvent(.)\n");
    #endif

    int r = real_XPeekEvent(display, event_return);
    handleEvent(event_return);
    return r;
}

int XWindowEvent(Display *display, Window w, long event_mask, XEvent *event_return) {
    #ifdef DEBUG
        fprintf(stderr, "XWindowEvent(.)\n");
    #endif

    int r = real_XWindowEvent(display, w, event_mask, event_return);
    handleEvent(event_return);
    return r;
}
