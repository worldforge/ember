AC_INIT(src/main/Dime.cpp)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(Dime, 0.1.3)

dnl Detect the canonical host and target build environment.
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AM_PROG_LIBTOOL
AC_PROG_CPP
AC_PROG_CXX
dnl AM_PROG_LEX

dnl Check for C++ libraries:
AC_LANG_CPLUSPLUS

dnl Check for the Boost shared_ptr.hpp shared pointer header
AC_CHECK_HEADER(boost/shared_ptr.hpp, ,AC_MSG_ERROR(The Boost C++ libraries are required to build Dime. They're available from www.boost.org))

dnl Check for C libraries:
AC_LANG_C

dnl Check for SDL and SDL_image
AM_PATH_SDL(1.2.0, , 
	AC_MSG_ERROR(libSDL >=1.2.0 is required to build Dime.  It's available from www.libsdl.org)
)

AC_CHECK_LIB(SDL_image, IMG_Load, , [
    AC_MSG_ERROR(libSDL_image not found, please install it.  It can be found at www.libsdl.org under "libraries")
])

CFLAGS="$CFLAGS $SDL_CFLAGS -Wall"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS -Wall"
LDFLAGS="$LDFLAGS $SDL_LIBS"

dnl Check for SigC++
AM_PATH_SIGC(1.0.3,[
       CXXFLAGS="$CXXFLAGS $SIGC_CFLAGS"
       LDFLAGS="$LDFLAGS $SIGC_LIBS"
       ],
AC_MSG_ERROR(Couldn't find libsigc++ config [or config failed])
)

dnl Check for Atlas
AM_PATH_ATLAS(0.4.4 ,[
    CXXFLAGS="$CXXFLAGS $ATLAS_CFLAGS"
    LDFLAGS="$LDFLAGS $ATLAS_LIBS"
    ],
AC_MSG_ERROR(Couldn't find Atlas [or tests failed])
)

dnl Check for Eris
AM_PATH_ERIS(0.9.5 ,[
    CXXFLAGS="$CXXFLAGS $ERIS_CFLAGS"
    LDFLAGS="$LDFLAGS $ERIS_LIBS"
    ],
AC_MSG_ERROR(Couldn't find Eris [or tests failed])
)

dnl Check for Varconf
AM_PATH_VARCONF(0.5.2 ,[
    CXXFLAGS="$CXXFLAGS $VARCONF_CFLAGS"
    LDFLAGS="$LDFLAGS $VARCONF_LIBS"
    ],
AC_MSG_ERROR(Couldn't find Varconf [or tests failed])
)


dnl Check for the FreeType 2 library
dnl
dnl Get the cflags and libraries from the freetype-config script
dnl
AC_ARG_WITH(freetype-prefix,[  --with-freetype-prefix=PFX   Prefix where FREETYPE is 
installed (optional)],
            freetype_prefix="$withval", freetype_prefix="")
AC_ARG_WITH(freetype-exec-prefix,[  --with-freetype-exec-prefix=PFX Exec prefix 
where FREETYPE is installed (optional)],
            freetype_exec_prefix="$withval", freetype_exec_prefix="")

if test x$freetype_exec_prefix != x ; then
     freetype_args="$freetype_args --exec-prefix=$freetype_exec_prefix"
     if test x${FREETYPE_CONFIG+set} != xset ; then
        FREETYPE_CONFIG=$freetype_exec_prefix/bin/freetype-config
     fi
fi
if test x$freetype_prefix != x ; then
     freetype_args="$freetype_args --prefix=$freetype_prefix"
     if test x${FREETYPE_CONFIG+set} != xset ; then
        FREETYPE_CONFIG=$freetype_prefix/bin/freetype-config
     fi
fi
AC_PATH_PROG(FREETYPE_CONFIG, freetype-config, no)
no_freetype=""
if test "$FREETYPE_CONFIG" = "no" ; then
    AC_MSG_ERROR([
*** Unable to find FreeType2 library (http://www.freetype.org/)
])
else
    CXXFLAGS="$CXXFLAGS `$FREETYPE_CONFIG $freetypeconf_args --cflags`"
    LIBS="$LIBS `$FREETYPE_CONFIG $freetypeconf_args --libs`"
fi

dnl Cppunit check

found_cppunit=no

AM_PATH_CPPUNIT(1.8.0, [
  AC_MSG_RESULT(
*** Found CppUnit
*** Setting up make check to run tests...)
	CXXFLAGS="$CXXFLAGS -DUSE_CPP_UNIT"
	LIBS="$LIBS $CPPUNIT_LIBS"
], 
  AC_MSG_RESULT([

*** Unable to find a recent enough CppUnit
*** Will not run unit testing!!
*** 
*** To get CppUnit see http://cppunit.sourceforge.net
]))

dnl OpenGL and GLU check


AC_CHECK_LIB(GL,
	     glViewport,
	     LIBS="$LIBS -lGL",
	     AC_MSG_ERROR([
	     Unable to find An OpenGL environment
	     ])
	)
AC_CHECK_LIB(GLU,
	     gluPerspective,
	     LIBS="$LIBS -lGLU",
	     AC_MSG_ERROR([
	     Unable to find the GLU library
	     ])
)


AC_LANG_CPLUSPLUS
AC_REQUIRE_CPP



AC_OUTPUT([
  Makefile
  src/Makefile
  src/framework/Makefile
  src/services/Makefile
  src/services/config/Makefile
  src/services/logging/Makefile
  src/services/test/Makefile
  src/services/metaserver/Makefile
  src/services/input/Makefile
  src/services/image/Makefile
  src/services/font/Makefile
  src/services/gui/Makefile
  src/services/platform/Makefile
  src/services/server/Makefile
  src/services/datamodel/Makefile
  test/Makefile
  src/main/Makefile
  test/Makefile
  dime.dox
  dime.spec
])



