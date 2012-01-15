CFLAGS = -std=c99 -Wall -Wextra -Wwrite-strings -Wunreachable-code -fPIC
DEBUGFLAGS = -g -DDEBUG

all:
	gcc $(CFLAGS) -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_X11.c wrapper_dlsym.c ./modes/*.c -ldl -include ./config.h

debug:
	gcc $(CFLAGS) $(DEBUGFLAGS) -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_X11.c wrapper_dlsym.c ./modes/*.c -ldl -include ./config.h

clean:
	rm glQuadBufferEmu.so
