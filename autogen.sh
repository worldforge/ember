#! /bin/sh

rm -f config.cache

if test -d /usr/local/share/aclocal ; then
	ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
fi
aclocal $ACLOCAL_FLAGS

if [[ $OSTYPE == *darwin* ]] ; then
LIBTOOLIZE="glibtoolize"
else
LIBTOOLIZE="libtoolize"
fi

$LIBTOOLIZE --force --copy

autoheader
automake --gnu --add-missing --copy
autoconf
