all:
	gcc -Wall -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_glut.c wrapper_X11.c wrapper_dlsym.c ./modes/*.c -ldl -lglut

clean:
	rm glQuadBufferEmu.so
