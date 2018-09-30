.PHONY: art.so main debug

all: art.so main debug

build:
	mkdir build

art.so: art.c build
	cc -o build/$@ -nostdlib -ggdb -O2 -shared -fPIC $<

debug: main_debug.c build
	cc -o build/$@ -ggdb -O2 $< -lX11 -Wall -ldl

main: main_x11.c build
	cc -o build/$@ -ggdb -O2 $< -lX11 -Wall
