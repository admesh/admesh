ACLOCAL_AMFLAGS = -I m4

AM_LDFLAGS = \
	-Wl,--gc-sections \
	-Wl,--as-needed \
	-no-undefined

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libadmesh.pc

EXTRA_DIST = \
	admesh-doc.txt \
	COPYING \
	README \
	INSTALL \
	ChangeLog.old \
	libadmesh.pc.in	

CLEANFILES = libadmesh.pc

bin_PROGRAMS = admesh
admesh_SOURCES = \
	src/admesh.c \
	src/connect.c \
	src/normals.c \
	src/shared.c \
	src/util.c
admesh_LDADD = \
	libadmesh.la

# libadmesh libtool versioning
LIBADMESH_CURRENT=1
LIBADMESH_REVISION=0
LIBADMESH_AGE=0

pkginclude_HEADERS = src/stl.h
lib_LTLIBRARIES = libadmesh.la

libadmesh_la_SOURCES = \
	src/stlinit.c \
	src/stl_io.c
libadmesh_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBADMESH_CURRENT):$(LIBADMESH_REVISION):$(LIBADMESH_AGE)

distclean-local:
	rm -rf *.cache *~

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog
