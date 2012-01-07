#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"

typedef void *(*dlsym_prototype)(void *, const char *);

void *dlsym(void *handle, const char *symbol) {
	static void *hd;
	static dlsym_prototype real_dlsym = NULL;

	void *r;
	
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

	r = findWrapFunction(symbol);
	if( r != NULL ) {
		return r;
	} else {
		return (real_dlsym (handle, symbol));
	}
}
