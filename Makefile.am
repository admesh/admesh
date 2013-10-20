ACLOCAL_AMFLAGS = -I m4

pkgconfdir = $(libdir)/pkgconfig

EXTRA_DIST = \
	ADMESH.DOC \
	COPYING \
	README \
	INSTALL \
	ChangeLog.old


bin_PROGRAMS = admesh
admesh_SOURCES = \
	src/admesh.c \
	src/connect.c \
	src/getopt.c \
	src/getopt1.c \
	src/normals.c \
	src/shared.c \
	src/stlinit.c \
	src/stl_io.c \
	src/util.c \
	src/getopt.h \
	src/stl.h

distclean-local:
	rm -rf *.cache *~

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog
