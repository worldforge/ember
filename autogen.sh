#! /bin/sh

rm -f config.cache

if test -d /usr/local/share/aclocal ; then
	ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
fi
aclocal $ACLOCAL_FLAGS
libtoolize --force --copy
autoheader
automake --gnu --add-missing --copy
autoconf
