/* Xlib.h is the default header that is included and has the core functionallity */
#include <X11/Xlib.h> 

/* Xatom.h includes functionallity for creating new protocol messages */
#include <X11/Xatom.h> 

/* keysym.h contains keysymbols which we use to resolv what keys that are being pressed */
#include <X11/keysym.h> 

/* printf */
#include <stdio.h> 

/* the XF86 Video Mode extension allows us to change the displaymode of the server

* this allows us to set the display to fullscreen and also read videomodes and   
* other information.                                                             
*/                                                                               
#include <X11/extensions/xf86vmode.h> 

/* gl.h we need OpenGL :-) */
#include <GL/gl.h> 
#include <GL/glx.h>
#include <GL/glu.h> 

#define WIDTH 640
#define HEIGHT 480
#define TITLE "OpenGL in X11" 

typedef struct {

   Display *dpy;
   int screen;  
   Window win;  
   GLXContext ctx;
   XSetWindowAttributes attr;
   Bool fs;                  
   Bool doubleBuffered;      
   XF86VidModeModeInfo deskMode;
   int x, y;                    
   unsigned int width, height;  
   unsigned int depth;                                                                                                                                        

} GLWindow;

GLWindow GLWin;

/* most important variable

* it contains information about the X server which we communicate with
*/                                                                    

Display * display; int screen; /* our window instance */ Window window; GLXContext context; XSetWindowAttributes winAttr; Bool fullscreen = False; Bool doubleBuffered; /* original desktop mode which we save so we can restore it later */ XF86VidModeModeInfo desktopMode; int x, y; unsigned int width, height; unsigned int depth;

GLfloat rotQuad = 0.0f;

/* attributes for a single buffered visual in RGBA format with at least

* 4 bits per color and a 16 bit depth buffer */                       

static int attrListSgl[] = {

   GLX_RGBA, GLX_RED_SIZE, 4,                                         
   GLX_GREEN_SIZE, 4,                                                 
   GLX_BLUE_SIZE, 4,                                                  
   GLX_DEPTH_SIZE, 16,                                                
   None                                                               

};

/* attributes for a double buffered visual in RGBA format with at least

* 4 bits per color and a 16 bit depth buffer */                       

static int attrListDbl[] = {

   GLX_RGBA, GLX_DOUBLEBUFFER,                                        
   GLX_RED_SIZE, 4,                                                   
   GLX_GREEN_SIZE, 4,                                                 
   GLX_BLUE_SIZE, 4,                                                  
   GLX_DEPTH_SIZE, 16,                                                
   None                                                               

};

/* prototypes */ void createWindow(); void destroyWindow(); void resizeGL(unsigned int, unsigned int); void initGL();

/*

* create a window
*/               

void createWindow() {

   XVisualInfo *vi;
   Colormap cmap;  
   int i, dpyWidth, dpyHeight;
   int glxMajor, glxMinor, vmMajor, vmMinor;
   XF86VidModeModeInfo **modes;             
   int modeNum, bestMode;                   
   Atom wmDelete;                           
   Window winDummy;                         
   unsigned int borderDummy;                

   /* set best mode to current */
   bestMode = 0;                 
   /* get a connection */        
   display = XOpenDisplay(0);    
   screen = DefaultScreen(display);
   XF86VidModeQueryVersion(display, &vmMajor, &vmMinor);
   printf("XF86 VideoMode extension version %d.%d\n", vmMajor, vmMinor);
   XF86VidModeGetAllModeLines(display, screen, &modeNum, &modes);       
   /* save desktop-resolution before switching modes */                 
   GLWin.deskMode = *modes[0];
   desktopMode = *modes[0];                                      
   /* look for mode with requested resolution */                        
   for (i = 0; i < modeNum; i++)                                        
   {                                                                    
       if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
           bestMode = i;                                                   
   }                                                                       
   /* get an appropriate visual */                                         
   vi = glXChooseVisual(display, screen, attrListDbl);                     
   if (vi == NULL)                                                         
   {                                                                       
       vi = glXChooseVisual(display, screen, attrListSgl);                 
       doubleBuffered = False;                                             
       printf("singlebuffered rendering will be used, no doublebuffering available\n");
   }                                                                                   
   else                                                                                
   {                                                                                   
       doubleBuffered = True;                                                          
       printf("doublebuffered rendering available\n");                                 
   }                                                                                   
   glXQueryVersion(display, &glxMajor, &glxMinor);                                     
   printf("GLX-Version %d.%d\n", glxMajor, glxMinor);                                  
   /* create a GLX context */                                                          
   context = glXCreateContext(display, vi, 0, GL_TRUE);                                
   /* create a color map */                                                            
   cmap = XCreateColormap(display, RootWindow(display, vi->screen),                    
       vi->visual, AllocNone);                                                         
   winAttr.colormap = cmap;                                                            
   winAttr.border_pixel = 0;                                                           

   if (fullscreen)
   {              
       /* switch to fullscreen */
       XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
       XF86VidModeSetViewPort(display, screen, 0, 0);            
       dpyWidth = modes[bestMode]->hdisplay;                     
       dpyHeight = modes[bestMode]->vdisplay;                    
       printf("resolution %dx%d\n", dpyWidth, dpyHeight);        
       XFree(modes);                                             

       /* set window attributes */
       winAttr.override_redirect = True;
       winAttr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
           StructureNotifyMask;                                            
       window = XCreateWindow(display, RootWindow(display, vi->screen),    
           0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
           CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,   
           &winAttr);                                                       
       XWarpPointer(display, None, window, 0, 0, 0, 0, 0, 0);               
               XMapRaised(display, window);                                 
       XGrabKeyboard(display, window, True, GrabModeAsync,                  
           GrabModeAsync, CurrentTime);                                     
       XGrabPointer(display, window, True, ButtonPressMask,                 
           GrabModeAsync, GrabModeAsync, window, None, CurrentTime);        
   }                                                                        
   else                                                                     
   {                                                                        
       /* create a window in window mode*/                                  
       winAttr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | 
           StructureNotifyMask;                                             
       window = XCreateWindow(display, RootWindow(display, vi->screen),     
           0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,      
           CWBorderPixel | CWColormap | CWEventMask, &winAttr);             
       /* only set window title and handle wm_delete_events if in windowed mode */
       wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);                 
       XSetWMProtocols(display, window, &wmDelete, 1);                            
       XSetStandardProperties(display, window, TITLE,                             
           TITLE, None, NULL, 0, NULL);                                           
       XMapRaised(display, window);                                               
   }                                                                              
   /* connect the glx-context to the window */                                    
   glXMakeCurrent(display, window, context);                                      
   if (glXIsDirect(display, context))                                             
       printf("DRI enabled\n");                                                   
   else                                                                           
       printf("no DRI available\n");                                              
   initGL();                                                                      

}

/*

* destroy the window
*/                  

void destroyWindow() {

   if( context )    
   {                
       if( !glXMakeCurrent(display, None, NULL))
       {                                        
           printf("Could not release drawing context.\n");
       }                                                  
       /* destroy the context */                          
       glXDestroyContext(display, context);               
       context = NULL;                                    
   }                                                      
   /* switch back to original desktop resolution if we were in fullscreen */
   if( fullscreen )                                                         
   {                                                                        
       XF86VidModeSwitchToMode(display, screen, &desktopMode);              
       XF86VidModeSetViewPort(display, screen, 0, 0);                       
   }                                                                        
   XCloseDisplay(display);                                                  

}

void resizeGL(unsigned int width, unsigned int height) {

   /* prevent divide-by-zero */                      
   if (height == 0)                                  
       height = 1;                                   
   glViewport(0, 0, width, height);                  
   glMatrixMode(GL_PROJECTION);                      
   glLoadIdentity();                                 
   gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);                                           

}

void initGL() {

   glShadeModel(GL_SMOOTH);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth(1.0f);                  
   glEnable(GL_DEPTH_TEST);             
   glDepthFunc(GL_LEQUAL);              
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /* we use resizeGL once to set up our initial perspective */
   resizeGL(width, height);                                    
   /* Reset the rotation angle of our object */                
   rotQuad = 0;                                                
   glFlush();                                                  

}

void renderGL() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();                                  
   glTranslatef(0.0f, 0.0f, -7.0f);                   
   glRotatef(rotQuad, 1.0f, 0.5f, 0.25f);             
   glBegin(GL_QUADS);                                 
       /* top of cube */                              
       glColor3f(0.0f, 1.0f, 0.0f);                   
       glVertex3f(1.0f, 1.0f, -1.0f);                 
       glVertex3f(-1.0f, 1.0f, -1.0f);                
       glVertex3f(-1.0f, 1.0f, 1.0f);                 
       glVertex3f(1.0f, 1.0f, 1.0f);                  
       /* bottom of cube */                           
       glColor3f(1.0f, 0.5f, 0.0f);                   
       glVertex3f(1.0f, -1.0f, 1.0f);                 
       glVertex3f(-1.0f, -1.0f, 1.0f);                
       glVertex3f(-1.0f, -1.0f, -1.0f);               
       glVertex3f(1.0f, -1.0f, -1.0f);                
       /* front of cube */                            
       glColor3f(1.0f, 0.0f, 0.0f);                   
       glVertex3f(1.0f, 1.0f, 1.0f);                  
       glVertex3f(-1.0f, 1.0f, 1.0f);                 
       glVertex3f(-1.0f, -1.0f, 1.0f);                
       glVertex3f(1.0f, -1.0f, 1.0f);                 
       /* back of cube */                             
       glColor3f(1.0f, 1.0f, 0.0f);                   
       glVertex3f(-1.0f, 1.0f, -1.0f);                
       glVertex3f(1.0f, 1.0f, -1.0f);                 
       glVertex3f(1.0f, -1.0f, -1.0f);                
       glVertex3f(-1.0f, -1.0f, -1.0f);               
       /* right side of cube */                       
       glColor3f(1.0f, 0.0f, 1.0f);                   
       glVertex3f(1.0f, 1.0f, -1.0f);                 
       glVertex3f(1.0f, 1.0f, 1.0f);                  
       glVertex3f(1.0f, -1.0f, 1.0f);                 
       glVertex3f(1.0f, -1.0f, -1.0f);                
       /* left side of cube */                        
       glColor3f(0.0f, 1.0f, 1.0f);                   
       glVertex3f(-1.0f, 1.0f, 1.0f);                 
       glVertex3f(-1.0f, 1.0f, -1.0f);                
       glVertex3f(-1.0f, -1.0f, -1.0f);               
       glVertex3f(-1.0f, -1.0f, 1.0f);                
   glEnd();                                           
   rotQuad += 0.1f;                                   
   /* swap the buffers if we have doublebuffered */   
   if (doubleBuffered)                                
   {                                                  
       glXSwapBuffers(display, window);               
   }                                                  

}

int main(int argc, char ** argv) {

   XEvent event;               
   Bool done = False;          

   width = WIDTH;
   height = HEIGHT;

   createWindow();

   /* wait for events and eat up cpu. ;-) */
   while (!done)                            
   {                                        
       /* handle the events in the queue */ 
       while (XPending(display) > 0)        
       {                                    
           XNextEvent(display, &event);     
           switch (event.type)              
           {                                
               case Expose:                 
                       if (event.xexpose.count != 0)
                           break;                   
                   renderGL();                      
                       break;                       
                   case ConfigureNotify:            
                   /* call resizeGL only if our window-size changed */
                       if ((event.xconfigure.width != GLWin.width) ||
                           (event.xconfigure.height != GLWin.height))
                       {
                           width = event.xconfigure.width;
                           height = event.xconfigure.height;
                           resizeGL(width, height);
                       }
                       break;
               /* exit in case of a mouse button press */
               case ButtonPress:
                   done = True;
                   break;
               case KeyPress:
                   if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
                   {
                       done = True;
                   }
                   if (XLookupKeysym(&event.xkey,0) == XK_F1)
                   {
                       destroyWindow();
                       fullscreen = !fullscreen;
                       createWindow();
                   }
                   break;
               case ClientMessage:
                   if (strcmp(XGetAtomName(display, event.xclient.message_type),
                              "WM_PROTOCOLS") == 0)
                   {
                       done = True;
                   }
                   break;
               default:
                   break;
           }
       }
       renderGL();
   }

   destroyWindow();

   return 0;

} 
