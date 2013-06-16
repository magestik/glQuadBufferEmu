INSTALL_DIR=/usr/local/lib/glQuadBufferEmu

all: glQuadBufferEmu tests

tests:
	cd tests; make

glQuadBufferEmu:
	cd src; make

install: glQuadBufferEmu
	mkdir -p $(INSTALL_DIR)
	cp ./src/glQuadBufferEmu.so $(INSTALL_DIR)/libGL.so
	ln -f -s $(INSTALL_DIR)/libGL.so $(INSTALL_DIR)/libGL.so.1
	echo $(INSTALL_DIR) > ./conf/glQuadBufferEmu.conf
	cp ./conf/71-stereoscopic.rules /etc/udev/rules.d/
	cp ./conf/glQuadBufferEmu.conf /etc/ld.so.conf.d/
	ldconfig

clean:
	rm glQuadBufferEmu.so
	cd src; make clean
	cd tests; make clean
