#! /bin/sh

SHELL = /bin/sh
CC = @CC@
CFLAGS = -I@srcdir@ @CFLAGS@
CPPFLAGS = @CPPFLAGS@
LDFLAGS = @LDFLAGS@
LIBOBJS = @LIBOBJS@
.SUFFIXES:
.SUFFIXES: .c .o

OBJ = admesh.o connect.o stl_io.o stlinit.o util.o normals.o shared.o @LIBOBJS@

all: @srcdir@/configure $(OBJ)
	$(CC) @LDFLAGS@ -o admesh $(OBJ) @LIBS@

clean:
	rm -f $(OBJ) admesh core admesh.core core.admesh
	rm -f config.h config.log config.status\
	      config.cache Makefile

