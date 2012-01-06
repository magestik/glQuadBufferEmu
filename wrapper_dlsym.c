#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "wrappers.h"


void *dlsym(void *handle, const char *symbol) {
	//if(DEBUG) 
	fprintf(stderr, "dlsym(%s)\n", symbol);
		
	void *result = __libc_dlsym(handle, symbol); // the replacement we are going to use
	return result;
}
