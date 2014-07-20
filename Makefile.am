ACLOCAL_AMFLAGS = -I m4

AM_LDFLAGS = @DEAD_STRIP@
AM_LDFLAGS += -no-undefined

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libadmesh.pc

doc_DATA = \
	admesh-doc.txt \
	README.md \
	COPYING \
	AUTHORS \
	ChangeLog \
	ChangeLog.old

EXTRA_DIST = \
	$(doc_DATA) \
	INSTALL \
	libadmesh.pc.in \
	block.stl

CLEANFILES = libadmesh.pc

dist_man_MANS = admesh.1

bin_PROGRAMS = admesh
admesh_SOURCES = \
	src/admesh.c
admesh_LDADD = \
	libadmesh.la

# libadmesh libtool versioning
LIBADMESH_CURRENT=1
LIBADMESH_REVISION=0
LIBADMESH_AGE=0

pkginclude_HEADERS = src/stl.h
lib_LTLIBRARIES = libadmesh.la

libadmesh_la_SOURCES = \
	src/connect.c \
	src/normals.c \
	src/shared.c \
	src/stlinit.c \
	src/stl_io.c \
	src/util.c

libadmesh_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBADMESH_CURRENT):$(LIBADMESH_REVISION):$(LIBADMESH_AGE)

distclean-local:
	rm -rf *.cache *~

AUTHORS:
	git log --format='%aN <%aE>' | sort -u > $@

ChangeLog:
	./fortag.sh >$@

dist-hook: AUTHORS ChangeLog
