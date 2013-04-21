#! /bin/sh

rm -f config.cache

#Check if the autoreconf command is available, and use that if so.
if command -v autoreconf >/dev/null 2>&1 ; then
  echo autoreconf...
  autoreconf --install
else
  if test -d /usr/local/share/aclocal ; then
    ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
  fi

  (command -v aclocal) < /dev/null > /dev/null 2>&1 || {
      echo aclocal not found
      exit 1
  }
  echo aclocal...
  aclocal -I m4 $ACLOCAL_FLAGS

  #The GNU libtoolize is called 'glibtoolize' on Darwin.
  if [ "`echo $OSTYPE | grep darwin`" != "" ] ; then
    LIBTOOLIZE="glibtoolize"
  else
    LIBTOOLIZE="libtoolize"
  fi

  (command -v $LIBTOOLIZE) < /dev/null > /dev/null 2>&1 || {
      echo $LIBTOOLIZE not found
      exit 1
  }
  echo $LIBTOOLIZE...
  $LIBTOOLIZE --force --copy

  (command -v autoheader) < /dev/null > /dev/null 2>&1 || {
      echo autoheader not found
      exit 1
  }
  echo autoheader...
  autoheader

  (command -v automake) < /dev/null > /dev/null 2>&1 || {
      echo automake not found
      exit 1
  }
  echo automake...
  automake --gnu --add-missing --copy

  (command -v autoconf) < /dev/null > /dev/null 2>&1 || {
      echo autoconf not found
      exit 1
  }
  echo autoconf...
  autoconf
fi
