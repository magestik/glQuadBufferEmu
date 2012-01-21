/*******************************************************************************
 * wrapper_dlsym.c
 * You can compile this as a shared library.
 * compile with this line:
 * gcc -ldl -Wl,-init=dlsym_init -shared -o wrapper_dlsym.la wrapper_dlsym.c
 */


/*
#define FATAL_ERROR (str_err)\
    {fprintf (stderr,"[EE] %s",str_err); exit(EXIT_FAILURE)}
*/

#include "wrapper_dlsym.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/* __lib_dlsym prototype as it is not defined in dlfcn.h */
void* __libc_dlsym(void *__map, __const char *__name);

/* wrapped dlsym functions pointers */
static void* dlsym_hd = NULL;

/* and the real one */
static DLSYM_PROTOTYPE real_dlsym = NULL;

/* a NULL terminated chained list of HANDLE_PAIR
 * used by dlsym__find_function to find the corresponding function */
static HANDLE_PAIR* Handles = NULL;


/* initiate wrapper_dlsym.so */
void dlsym_init (void)
{
    dlsym_hd = dlsym_open_lib ("libdl.so");

    if ((real_dlsym = __libc_dlsym (dlsym_hd, "dlsym")) != NULL)
    {
        fprintf (stderr, "[**] __libc_dlsym() ... Ok\n");
    }
    else
    {
        FATAL_ERROR ("[**] __libc_dlsym() ... Failure");
    }
    fprintf (stderr, "[**] dlsym wrapper initialized.\n");
}


/* Returning our wrap function to dlsym or glXGetProcAdress*/
void *dlsym_find_function (const char *symbol)
{
    void* ret = NULL;
    HANDLE_PAIR* cur = Handles;

    while (cur != NULL && ret == NULL)
    {
        if( !strcmp(cur->symbol, symbol) )
        {
            ret = cur->handle;
        }
        cur = cur->nxt;
    }

    return ret;
}


/* open a library with dlopen with error checking */
void* dlsym_open_lib (const char* lib)
{
    void* r = NULL;

    fprintf (stderr, "[**] Opening library \"%s\"\n", lib);
    if ((r = dlopen (lib, RTLD_LAZY)) == NULL)
    {
        FATAL_ERROR (dlerror());
    }

    return r;
}


/* dlsym wrapper */
void* dlsym (void *handle, const char *symbol)
{
    void *r = dlsym_find_function (symbol);

    if ((dlsym_hd == NULL) || (real_dlsym == NULL))
        dlsym_init ();

    #ifdef DEBUG
    fprintf (stderr, "dlsym (%s) ... ", symbol);
    if (r != NULL)
    {
        printf ("Wrapped\n");
    }
    else
    {
        fprintf (stderr, "real_dlsym ");
        r = real_dlsym (handle, symbol);
        fprintf (stderr, (r != NULL)? "Ok\n" : "Failed\n");
    }

    return r;

    #else
    return r? r : real_dlsym (handle, symbol);

    #endif
}


/* dlsym with error checking */
void *dlsym_test (void *lib, const char *name)
{
    #ifdef DEBUG
    fprintf (stderr, "dlsym_test (%p, %s) ... ", lib, name);
    #endif
    void *function = __libc_dlsym(lib, name);

    if (function == NULL)
    {
        FATAL_ERROR (dlerror());
    }
    #ifdef DEBUG
    else
    {
        fprintf (stderr, "Ok\n");
    }
    #endif

    return function;
}


void dlsym_add_wrap (void* func, const char* symbol)
{
    HANDLE_PAIR* cur = Handles;
    HANDLE_PAIR* last = NULL;

    if (cur == NULL)
    {
        Handles = malloc (sizeof (HANDLE_PAIR));
        Handles->handle = func;
        Handles->symbol = symbol;
        Handles->nxt = NULL;
    }
    else
    {
        while (cur != NULL)
        {
            last = cur;
            cur = cur->nxt;
        }
        cur = malloc (sizeof (HANDLE_PAIR));
        cur->handle = func;
        cur->symbol = symbol;
        cur->nxt = NULL;
        last->nxt = cur;
    }
}
