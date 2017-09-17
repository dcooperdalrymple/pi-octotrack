CC=gcc
CXX=g++
CPPFLAGS=-std=c++11 -Wall
HEADERS=-Iinclude -Iexternals/libserial/src -Iexternals/libusb/libusb -Iexternals/alsa-lib/include
LDFLAGS=-Lsrc -Lexternals/libserial/src/.libs -Lexternals/libusb/libusb/.libs -Lexternals/alsa-lib/src/.libs
LDLIBS=-lserial -lusb -lasound

SRCS=$(wildcard src/*.cpp)
OBJS=$(subst src,obj,$(subst .cpp,.o,$(SRCS)))

.PHONY: all rpi debug debug-rpi doc cross-compile help clean distclean

all: CPPFLAGS+= -O2
all: bin/octotrack
    strip bin/octotrack

debug: CPPFLAGS+= -D DEBUG -g3 -ggdb
debug: bin/octotrack

rpi: CPPFLAGS+= -O2
rpi: cross-compile bin/octotrack
    $(CROSS_COMPILE)-strip bin/octotrack

debug-rpi: CPPFLAGS+= -D DEBUG -g3 -ggdb
debug-rpi: cross-compile bin/octotrack

doc:
    doxygen

cross-compile:
    $(eval PATH:=$(shell pwd)/externals/tolls/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin:$(PATH))
    $(eval CROSS_COMPILE=arm-linux-gnueabihf)
    $(eval CC=$(CROSS_COMPILE)-gcc)
    $(eval CXX=$(CROSS_COMPILE)-g++)
    $(eval ARCH=arm)

bin/octotrack: libserial libusb libasound $(OBJS)
    mkdir -p bin
    $(CXX) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS)

obj/%.o: src/%.cpp
    mkdir -p obj
    $(CXX) $(CPPFLAGS) $(LDFLAGS) $(HEADERS) -o $@ -c $<

libserial:
    cd externals/libserial; ./configure --host=$(CROSS_COMPILE)
    make -C externals/libserial
    touch $@

libasound:
    cd externals/alsa-lib; libtoolize --force --copy --automake
    cd externals/alsa-lib; aclocal
    cd externals/alsa-lib; autoheader
    cd externals/alsa-lib; automake --foreign --copy --add-missing
    cd externals/alsa-lib; autoconf
    cd externals/alsa-lib; ./configure --host=$(CROSS_COMPILE) --disable-python
    make -C externals/alsa-lib
    touch $@

help:
    @echo "Possible targets:"
	@echo "	all 			Build synthberry natively"
	@echo "	debug 			Build synthberry natively with debugging options"
	@echo "	rpi 			Cross-compile synthberry for the Raspberry Pi"
	@echo "	debug-rpi 		Cross-compile synthberry for the Raspberry Pi with debugging options"

clean:
	rm -rf bin obj

distclean: clean
	-make -C externals/libserial distclean
    -make -C externals/libusb distclean
	-make -C externals/alsa-lib distclean
	rm -rf libserial libusb libasound doc
