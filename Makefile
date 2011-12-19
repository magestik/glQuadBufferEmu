all: lib

lib:
	gcc -ldl -lglut -Wall -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_glut.c ./modes/*.c

clean:
	rm glQuadBufferEmu.so
