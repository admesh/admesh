#!/bin/sh

if [ ! -e ltmain.sh ]; then
    libtoolize --force --copy
fi

aclocal -I m4
autoheader
automake -a -c --foreign
autoconf
