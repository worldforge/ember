#! /bin/sh

aclocal -I /usr/local/share/aclocal
libtoolize --force --copy
autoheader
automake --foreign --add-missing --copy
autoconf
