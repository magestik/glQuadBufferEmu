#ifndef H__WRAPPER_DLSYM
#define H__WRAPPER_DLSYM

#include "glQuadBufferEmu.h"

extern void* __libc_dlsym(void *__map, __const char *__name);
typedef void* (*dlsym_prototype)(void *, const char *);

void *dlsym(void *handle, const char *symbol);
void *dlsym_test(void *lib, const char *name);

#endif /* H__WRAPPER_DLSYM */
