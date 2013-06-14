INSTALL_DIR=/usr/local/lib/glQuadBufferEmu

all: glQuadBufferEmu tests

tests:
	cd tests; make

glQuadBufferEmu:
	cd src; make && mv ./glQuadBufferEmu.so ../

install: glQuadBufferEmu
	mkdir -p $(INSTALL_DIR)
	cp ./glQuadBufferEmu.so $(INSTALL_DIR)/libGL.so
	ln -s $(INSTALL_DIR)/libGL.so $(INSTALL_DIR)/libGL.so.1
	ln -s $(INSTALL_DIR)/libGL.so $(INSTALL_DIR)/libGL.so.1.0
	echo $(INSTALL_DIR) > ./glQuadBufferEmu.conf
	cp ./glQuadBufferEmu.conf /etc/ld.so.conf.d/
	ldconfig

clean:
	rm glQuadBufferEmu.so
	cd src; make clean
	cd tests; make clean
