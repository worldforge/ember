AC_INIT(src/main/Dime.cpp)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(Dime, 0.1.3)

dnl Detect the canonical host and target build environment.
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_PROG_LIBTOOL
AC_PROG_CPP
AC_PROG_CXX
dnl AM_PROG_LEX

dnl Allow maintainer mode
AM_MAINTAINER_MODE

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
SIGC_VERSION=1.0.4
PKG_CHECK_MODULES(SIGC, sigc++-1.2 >= 1.2.1,
     [
         CXXFLAGS="$CXXFLAGS $SIGC_CFLAGS"
         LDFLAGS="$LDFLAGS $SIGC_LIBS"
     ],
     [
        AM_PATH_SIGC($SIGC_VERSION,[
                CXXFLAGS="$CXXFLAGS $SIGC_CFLAGS"
                LDFLAGS="$LDFLAGS $SIGC_LIBS"
                ],
                AC_MSG_ERROR(Couldn't find sigc++ or insufficient version found. Please goto http://sourceforge.net/projects/libsigc/ and get at least version $SIGC_VERSION)
        )
     AC_MSG_WARN(Please note the we will drop support for this version of SigC++ eventually)
     ])

dnl Check for Atlas
AM_PATH_ATLAS(0.4.5 ,[
    CXXFLAGS="$CXXFLAGS $ATLAS_CFLAGS"
    LDFLAGS="$LDFLAGS $ATLAS_LIBS"
    ],
AC_MSG_ERROR(Couldn't find Atlas [or tests failed])
)

dnl Check for Eris
AM_PATH_ERIS(0.9.6 ,[
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
dnl 
AC_ARG_ENABLE(cppunit,[  --enable-cppunit     enables cppunit tests [default=yes]],
[ 
   if test x$enableval = xno; then 
		ac_use_cppunit="no"	
   else 
		ac_use_cppunit="yes"
   fi
], [ac_use_cppunit="yes"])

found_cppunit=no

if test "$ac_use_cppunit" = "yes"; then
AM_PATH_CPPUNIT(1.8.0, [
  AC_MSG_RESULT([
*** Found CppUnit
*** Setting up make check to run tests...])
	CXXFLAGS="$CXXFLAGS -DUSE_CPP_UNIT"
	LIBS="$LIBS $CPPUNIT_LIBS"
], 
  AC_MSG_RESULT([

*** Unable to find a recent enough CppUnit
*** Will not run unit testing!!
*** 
*** To get CppUnit see http://cppunit.sourceforge.net
]))
fi

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

dnl OpenAL check

AC_CHECK_LIB(openal,
	alutInit ,
	     LIBS="$LIBS -lopenal",
	     AC_MSG_ERROR([
	     Unable to find OpenAL (Open Audio Layer)
	     ])
	)

dnl debugging option
AC_ARG_ENABLE(debug,[  --enable-debug     creates debugging code [default=no]],
[ 
   if test x$enableval = xno; then 
		ac_use_debug_code="no"	
   else 
		ac_use_debug_code="yes"
   fi
], [ac_use_debug_code="no"])

if test "$ac_use_debug_code" = "yes"; then
      CFLAGS="$CFLAGS -DDEBUG"
      CXXFLAGS="$CXXFLAGS -DDEBUG"
else
      CFLAGS="$CFLAGS -DNDEBUG"
      CXXFLAGS="$CXXFLAGS -DNDEBUG"
fi

dnl gprof debugging option
AC_ARG_ENABLE(gprof,[  --enable-gprof     Add flags for gprof [default=no]],
[
   if test x$enableval = xno; then 
		ac_use_gprof_code="no"	
   else 
		ac_use_gprof_code="yes"
   fi
], [ac_use_gprof_code="no"])

if test "$ac_use_gprof_code" = "yes"; then
      CFLAGS="$CFLAGS -pg -g -a"
      CXXFLAGS="$CXXFLAGS -pg -g -a"
fi

dnl Test for libxml2
AM_PATH_XML2(2.4.24,[
    CFLAGS="$CFLAGS $XML_CFLAGS"
    CXXFLAGS="$CXXFLAGS $XML_CFLAGS"
    LDFLAGS="$LDFLAGS $XML_LIBS"
    ],
AC_MSG_ERROR(Couldn't find LibXML2 [or tests failed])
)

AC_LANG_CPLUSPLUS
AC_REQUIRE_CPP



AC_OUTPUT([
  Makefile
  bin/Makefile
  src/Makefile
  src/components/Makefile
  src/components/debug/Makefile
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
  src/services/gui/widget/Makefile
  src/services/platform/Makefile
  src/services/server/Makefile
  src/services/sound/Makefile
  src/services/datamodel/Makefile
  test/Makefile
  src/main/Makefile
  dime.dox
  dime.spec
])
