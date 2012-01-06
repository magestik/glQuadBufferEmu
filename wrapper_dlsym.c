#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

extern void *__libc_dlsym(void *__map, __const char *__name); 

void *dlsym(void *handle, const char *symbol) {
	fprintf(stderr, "dlsym(%s)\n", symbol);
	return __libc_dlsym(handle, symbol);
}

// gcc -Wall -fPIC -shared -o dlsymWrapper.so dlsym.c -ldl
