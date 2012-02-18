#include "glQuadBufferEmu.h"

/** Implementation of X11 functions *********************************************/

void handleEvent(XEvent *e)
{
    char buffer[10];

    switch(e->type)
    {
    case ConfigureNotify:
        #ifdef DEBUG
            fprintf (stderr, "ConfigureNotify (%d, %d)\n",
                     QBState.width, QBState.height);
        #endif

        QBState.height = e->xconfigure.height;
        QBState.width = e->xconfigure.width;
        break;

    case KeyPress:
        #ifdef DEBUG
            /* TODO: check "Print Screen" and "Escape" */
            fprintf (stderr, "Key pressed\n");
        #endif

        XLookupString (&e->xkey, buffer, sizeof(buffer), NULL, NULL);
        if (buffer[0] == 27)
        {
            fprintf (stderr, "[**] QuadBufferEmu Exit... Ouch!\n");
            exit (EXIT_SUCCESS);
        }
        break;
    /*
    default:
            fprintf(stderr, "Unknown event\n");
    */
    }
}

/* http://www.xfree86.org/current/XCreateWindow.3.html */
Window XCreateWindow
    (Display *display, Window parent, int x, int y, unsigned int width,
     unsigned int height, unsigned int border_width, int depth,
     unsigned int class, Visual *visual, unsigned long valuemask,
     XSetWindowAttributes *attributes)
{
    #ifdef DEBUG
        fprintf (stderr,
         "XCreateWindow (%p, %ld, %d, %d, %u, %u, %u, %d, %u, %p, %ld, %p)\n",
         display, parent, x, y, width, height, border_width, depth, class,
         visual, valuemask, attributes);
    #endif

    QBState.height = height;
    QBState.width = width;

    if (QBState.fullscreen == GL_TRUE) { // force fullscreen
		int scrnum = DefaultScreen (display);
		
		x = 0;
        y = 0;
        height = DisplayHeight (display, scrnum );
        width = DisplayWidth (display, scrnum );

        attributes->override_redirect = True;
        valuemask = valuemask | CWOverrideRedirect;
    }

    return real_XCreateWindow (display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
}

/* http://www.xfree86.org/current/XDestroyWindow.3.html */
int XDestroyWindow (Display *display, Window w)
{
    #ifdef DEBUG
        fprintf (stderr, "XDestroyWindow (%p, %ld)\n", display, w);
    #endif
    int ret;

    if (wrap_XDestroyWindow == NULL || QBState.enabled == GL_FALSE) {
        ret = real_XDestroyWindow(display, w);
    } else {
        ret = wrap_XDestroyWindow(display, w);
    }

    return ret;
}

/* http://www.xfree86.org/current/XNextEvent.3.html */
int XNextEvent(Display *display, XEvent *event_return)
{
    #ifdef DEBUG
        fprintf (stderr, "XNextEvent (%p, %p)\n", display, event_return);
    #endif
    
    int r = real_XNextEvent(display, event_return);
    handleEvent (event_return);
    return r;
}

int XPeekEvent(Display *display, XEvent *event_return)
{
    #ifdef DEBUG
        fprintf (stderr, "XPeekEvent (%p, %p)\n", display, event_return);
    #endif
    
    int r = real_XPeekEvent(display, event_return);
    handleEvent (event_return);
    return r;
}

int XWindowEvent
    (Display *display, Window w, long event_mask, XEvent *event_return)
{
    #ifdef DEBUG
        fprintf (stderr, "XWindowEvent (%p, %ld, %ld, %p)\n",
                 display, w, event_mask, event_return);
    #endif
    
    int r = real_XWindowEvent(display, w, event_mask, event_return);
    handleEvent (event_return);
    return r;
}
