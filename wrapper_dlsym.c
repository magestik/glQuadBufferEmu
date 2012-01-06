#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "wrappers.h"

typedef void *(*dlsym_prototype)(void *, const char *);

void *dlsym(void *handle, const char *symbol) {
	static void *hd;
	static dlsym_prototype real_dlsym = NULL;

	if (real_dlsym == NULL) {
		hd = dlopen ("libdl.so", RTLD_NOW);

		if (hd == NULL) {
			puts ("dlopen Failed");
			return (NULL);
		}

		real_dlsym = __libc_dlsym (hd, "dlsym");
		if (real_dlsym == NULL) {
			puts ("__libc_dlsym failed");
			return (NULL);
		}

		puts ("link dlsym is SUCCESS");
	}

	printf("dlsym(%s)\n", symbol);

	if( !strcmp( (const char *)symbol, "glDrawBuffer") ) {
		return glDrawBuffer;
	} else if( !strcmp( (const char *)symbol, "glGetIntegerv") ) {
		return glGetIntegerv;
	} else if( !strcmp( (const char *)symbol, "glutInitDisplayMode") ) {
		return glutInitDisplayMode;
	} else {
		return (real_dlsym (handle, symbol));
	}
}
