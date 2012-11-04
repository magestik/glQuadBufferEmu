all: glQuadBufferEmu tests

tests:
	cd tests; make

glQuadBufferEmu:
	cd src; make && mv ./glQuadBufferEmu.so ../

clean:
	rm glQuadBufferEmu.so
	cd src; make clean
	cd tests; make clean
