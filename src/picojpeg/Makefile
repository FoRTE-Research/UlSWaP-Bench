MAX_PRINTF_LEVEL = INTEGER

all : g_host g_jpegdata.h main.elf


g_host : host.c picojpeg.c picojpeg.h g_jpegdata.h
	gcc -O2 host.c picojpeg.c -o g_host

g_jpegdata.h : bintoc.c
	gcc -O2 bintoc.c -o g_bintoc
	./g_bintoc

more_clean :
	rm -rf g_host out.bmp g_bintoc g_jpegdata.h

OBJS = picojpeg.o picojpegtest.o

include ../../Makefile.mk