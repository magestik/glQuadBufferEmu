all:
	gcc -std=c99 -Wall -Wextra -Wwrite-strings -Wunreachable-code -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_X11.c wrapper_dlsym.c ./modes/*.c -ldl -include ./config.h

debug:
	gcc -std=c99 -g -DDEBUG -Wall -Wextra -Wwrite-strings -Wunreachable-code -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_X11.c wrapper_dlsym.c ./modes/*.c -ldl -include ./config.h

clean:
	rm glQuadBufferEmu.so
