#! /bin/sh

rm -f config.cache

#Check if the autoreconf command is available, and use that if so.
if command -v autoreconf >/dev/null 2>&1 ; then
  autoreconf --install
else
  if test -d /usr/local/share/aclocal ; then
	  ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
  fi
  aclocal -I m4 $ACLOCAL_FLAGS

  if [ "`echo $OSTYPE | grep darwin`" != "" ] ; then
  LIBTOOLIZE="glibtoolize"
  else
  LIBTOOLIZE="libtoolize"
  fi

  $LIBTOOLIZE --force --copy

  autoheader
  automake --gnu --add-missing --copy
  autoconf
fi
