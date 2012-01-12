#ifndef H__WRAPPER_X11
#define H__WRAPPER_X11

#include "glQuadBufferEmu.h"

/* Real functions */
Window (*real_XCreateWindow)
        (Display *display, Window parent, int x, int y, unsigned int width,
        unsigned int height, unsigned int border_width, int depth,
        unsigned int class, Visual *visual, unsigned long valuemask,
        XSetWindowAttributes *attributes);

int (*real_XDestroyWindow) (Display *display, Window w);

int (*real_XNextEvent) (Display *display, XEvent *event_return);

int (*real_XPeekEvent) (Display *display, XEvent *event_return);

int (*real_XWindowEvent)
        (Display *display, Window w, long event_mask, XEvent *event_return);

/* Wrapper functions */
Window (*wrap_XCreateWindow)
        (Display *display, Window parent, int x, int y, unsigned int width,
         unsigned int height, unsigned int border_width, int depth,
         unsigned int class, Visual *visual, unsigned long valuemask,
         XSetWindowAttributes *attributes);

int (*wrap_XDestroyWindow) (Display *display, Window w);

#endif /* H__WRAPPER_X11 */
