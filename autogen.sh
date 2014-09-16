#!/bin/sh

case `uname` in Darwin*) glibtoolize --copy ;;
*) libtoolize --force --copy ;; esac

aclocal -I m4
autoheader
automake -a -c --foreign
autoconf
