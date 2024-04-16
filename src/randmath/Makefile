MAX_PRINTF_LEVEL = MINIMAL

default : main.elf

g_randmath : randmath.c
	gcc -o g_randmath randmath.c

g_abcmath.c : g_randmath
	./g_randmath

OBJS = g_abcmath.o main.o

include ../../Makefile.mk

more_clean:
	rm -rf g_randmath g_abcmath.c
