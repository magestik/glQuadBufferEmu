all:
	gcc -Wall -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_X11.c ./modes/*.c -ldl

clean:
	rm glQuadBufferEmu.so
