#! /bin/sh

rm -f config.cache
export WANT_AUTOMAKE='1.9'
export WANT_AUTOCONF='2.5'

if test -d /usr/local/share/aclocal ; then
	ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
fi
aclocal $ACLOCAL_FLAGS
libtoolize --force --copy
autoheader
automake --gnu --add-missing --copy
autoconf
