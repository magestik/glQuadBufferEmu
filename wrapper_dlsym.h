#ifndef H__WRAPPER_DLSYM
#define H__WRAPPER_DLSYM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/* address of the handler function and its symbol as a const char*
 * to be used as a chained list */
typedef struct handlePair
{
    void*               handle;
    const char*         symbol;
    struct handlePair*  nxt;
} HANDLE_PAIR;

typedef void* (*DLSYM_PROTOTYPE)(void *, const char *);

void  dlsym_init (void);
void* open_lib (const char* lib);
void* dlsym (void *handle, const char *symbol);
void* dlsym_test(void *lib, const char *name);
void  dlsym_add_wrap (void* func, const char* symbol);
void* dlsym_find_function (const char *symbol);

#endif /* H__WRAPPER_DLSYM */

