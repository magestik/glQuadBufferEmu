all: lib demos

demos: fire ipers plane polygone pulsar simple triangle

fire:
	cd tests/fire/; make

ipers:
	cd tests/ipers/; make

plane:	
	cd tests/plane/; make

polygone:
	cd tests/polygone/; make

pulsar:
	cd tests/pulsar/; make

simple:
	cd tests/simple/; make

triangle:
	cd tests/triangle/; make

lib:
	gcc -ldl -lglut -Wall -fPIC -shared -Wl,-init,QuadBufferEmuInit -o glQuadBufferEmu.so main.c wrapper_gl.c wrapper_glX.c wrapper_glut.c wrapper_X11.c ./modes/*.c

clean:
	rm glQuadBufferEmu.so
	rm tests/*/test
