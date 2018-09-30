.PHONY: art.so main debug

all: art.so main debug

art.so: art.c
	cc -o art.so -ggdb -O2 -shared -fPIC $<

debug: main_debug.c
	cc -o $@ -ggdb -O2 $< -lX11 -Wall -ldl

main: main_x11.c
	cc -o $@ -ggdb -O2 $< -lX11 -Wall
