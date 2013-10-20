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
	admesh.c \
	connect.c \
	getopt.c \
	getopt1.c \
	normals.c \
	shared.c \
	stlinit.c \
	stl_io.c \
	util.c \
	getopt.h \
	stl.h

distclean-local:
	rm -rf *.cache *~

dist-hook:
	git log --date=short --pretty="format:@%cd  %an  <%ae>  [%H]%n%n%s%n%n%e%b" | sed -e "s|^\([^@]\)|\t\1|" -e "s|^@||" >$(distdir)/ChangeLog
