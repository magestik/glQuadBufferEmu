#include "glQuadBufferEmu.h"

/** Implementation of glX functions *******************************************/

void calcFPS()
{
    // TODO : calculer FPS
}


GLXFBConfig *glXChooseFBConfig
    (Display *dpy, int screen, const int *attrib_list, int *nelements)
{
    int wrapped_attribList[60]; // 60 is enough ?
    int requested = 0;
    GLXFBConfig* ret = NULL;

    int i = 0;

    while (attrib_list[i] != None)
    {
        if (attrib_list[i] == GLX_STEREO)
        {
            if (attrib_list[i+1] == True)
            {
                requested += 2;
            }
            else if (attrib_list[i+1] != False)
            {
                requested++;
            }
            else
            {
                // attrib_list[i+1] == False
                //QBState.enabled = GL_FALSE;
            }
        }
        else
        {
            wrapped_attribList[i-requested] = attrib_list[i];
        }
        i++;
    }
    wrapped_attribList[i-requested] = None;

    if (requested > 0)
    {
        QBState.enabled = GL_TRUE;
        fprintf (stderr, "glXChooseFBConfig(GLX_STEREO)\n");
    }
    else
    {
        //QBState.enabled = GL_FALSE;
        fprintf (stderr, "glXChooseFBConfig(.)\n");
    }

    if (wrap_glXChooseFBConfig == NULL || QBState.enabled == GL_FALSE)
    {
        ret = real_glXChooseFBConfig
                (dpy, screen, (const int *)wrapped_attribList, nelements);
    }
    else
    {
        ret = wrap_glXChooseFBConfig
                (dpy, screen, (const int *)wrapped_attribList, nelements);
    }

    return ret;
}


XVisualInfo *glXChooseVisual (Display *dpy, int screen, int *attribList)
{
    int wrapped_attribList[60] = {None};
    int requested = 0;
    XVisualInfo* ret;

    int i = 0;

    while (attribList[i] != None)
    {
        if (attribList[i] == GLX_STEREO)
        {
            requested++;
        }
        else
        {
            wrapped_attribList[i - requested] = attribList[i];
        }
        i++;
    }

    if (requested > 0)
    {
        QBState.enabled = GL_TRUE;
        fprintf (stderr, "glXChooseVisual(GLX_STEREO)\n");
    }
    else
    {
        /*QBState.enabled = GL_FALSE;*/
        fprintf (stderr, "glXChooseVisual(.)\n");
    }

    if (wrap_glXChooseVisual == NULL || QBState.enabled == GL_FALSE)
    {
        ret = real_glXChooseVisual(dpy, screen, wrapped_attribList);
    }
    else
    {
        ret = wrap_glXChooseVisual(dpy, screen, wrapped_attribList);
    }

    #ifdef DEBUG
    fprintf (stderr,
            "glXChooseVisual (Display:%p, screen:%d, attribList %p)\n"
            ">  returned: %p\n",dpy, screen, attribList, ret);
    #endif

    return ret;
}


int glXGetConfig(Display *dpy, XVisualInfo *vis, int attrib, int *value)
{
    int ret;

    if(attrib == GLX_STEREO)
    {
        *value = True;
        ret = 0;
    }
    else
    {
        ret = real_glXGetConfig(dpy, vis, attrib, value);
    }

    return ret;
}


int glXGetFBConfigAttrib
    (Display *dpy, GLXFBConfig config, int attribute, int *value)
{
    int ret;

    if(attribute == GLX_STEREO)
    {
        if(QBState.enabled == GL_TRUE)
        {
            *value = True;
        }
        else
        {
            *value = False;
        }
        ret = 0;
    }
    else
    {
        ret = real_glXGetFBConfigAttrib(dpy, config, attribute, value);
    }

    return ret;
}


void (*glXGetProcAddress(const GLubyte *procname)) (void)
{
    #ifdef DEBUG
        fprintf(stderr, "glXGetProcAddress(%s)\n", procname);
    #endif

    void *r = dlsym_find_function((const char *)procname);

    return (r != NULL)? r : real_glXGetProcAddress(procname);
}


void (*glXGetProcAddressARB(const GLubyte *procname)) (void)
{
    #ifdef DEBUG
        fprintf(stderr, "glXGetProcAddressARB(%s)\n", procname);
    #endif

    void *r = dlsym_find_function((const char *)procname);

    return (r != NULL)? r : real_glXGetProcAddressARB(procname);
}


void glXSwapBuffers(Display * dpy, GLXDrawable drawable)
{
    #ifdef DEBUG
        fprintf(stderr, "glXSwapBuffers(%p, %p)\n", dpy, &drawable);
        calcFPS();
    #endif

    if(wrap_glXSwapBuffers == NULL || QBState.enabled == GL_FALSE)
    {
        real_glXSwapBuffers(dpy, drawable);
    }
    else
    {
        wrap_glXSwapBuffers(dpy, drawable);
    }
}

/******************************************************************************/
