.PHONY: main

main: main_x11.c
	cc -o main -ggdb -O2 $< -lX11 -Wall
