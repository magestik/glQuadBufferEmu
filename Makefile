CFLAGS = -std=c99 -Wall -Wextra -Wwrite-strings -Wunreachable-code -fPIC -shared -Wl,-init=QuadBufferEmuInit
LDFLAGS = -ldl
DEBUGFLAGS = -g -DDEBUG
PRJNAME = glQuadBufferEmu
SOURCES = *.c ./modes/*.c

all:
	gcc $(CFLAGS) -o $(PRJNAME).so $(SOURCES) $(LDFLAGS) -include ./util.h
	chmod a-x glQuadBufferEmu.so

debug:
	gcc $(CFLAGS) $(DEBUGFLAGS) -o $(PRJNAME).so $(SOURCES) $(LDFLAGS) -include ./util.h
	chmod a-x glQuadBufferEmu.so

clean:
	rm $(PRJNAME).so
