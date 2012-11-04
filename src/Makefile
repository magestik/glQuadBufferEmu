CFLAGS = -I../include -std=c99 -Wall -Wextra -Wwrite-strings -Wunreachable-code -fPIC -shared -Wl,-init=QuadBufferEmuInit
LDFLAGS = -ldl -lusb
DEBUGFLAGS = -g -DDEBUG
PRJNAME = glQuadBufferEmu
SOURCES = *.c ./modes/*.c

all:
	gcc $(CFLAGS) -o $(PRJNAME).so $(SOURCES) $(LDFLAGS) -include util.h

debug:
	gcc $(CFLAGS) $(DEBUGFLAGS) -o $(PRJNAME).so $(SOURCES) $(LDFLAGS) -include util.h

clean:
	rm $(PRJNAME).so
