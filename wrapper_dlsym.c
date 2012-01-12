#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "wrapper_dlsym.h"

void *dlsym(void *handle, const char *symbol) {
    static void *hd;
    static dlsym_prototype real_dlsym = NULL;
    void *r = NULL;

    if (real_dlsym == NULL) {
        hd = dlopen ("libdl.so", RTLD_NOW);
        if (hd == NULL) {
            printf("dlopen Failed");
        } else {
            real_dlsym = __libc_dlsym (hd, "dlsym");
            if (real_dlsym == NULL) {
                printf("__libc_dlsym failed\n");
            } else {
                printf("link dlsym is SUCCESS\n");
            }
        }
    }
    printf("dlsym(%s)\n", symbol);
    r = QuadBufferEmuFindFunction(symbol);

    return (r != NULL)? r : real_dlsym (handle, symbol);
}

/* dlsym with error checking */
void *dlsym_test(void *lib, const char *name) {
    char *error;
    void *function = __libc_dlsym(lib, name);

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    return function;
}
