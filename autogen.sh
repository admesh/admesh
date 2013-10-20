#!/bin/sh

libtoolize --force --copy
aclocal -I m4
autoheader
automake -a -c --foreign
autoconf
