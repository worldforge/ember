## Process this file with autoconf to produce a configure script.
AC_INIT(Ember, 0.2.0, clients@worldforge.org)

AC_CONFIG_SRCDIR(src)

# Detect the canonical host and target build environment.
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE
#(Ember, 0.2.0, clients@worldforge.org)
AM_CONFIG_HEADER(config.h)

AC_PREREQ(2.50)
AC_COPYRIGHT([This script is under the GPL and is owned by the people in AUTHORS])


# Checks for Programs
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
dnl AM_PROG_LEX

# Allow maintainer mode
AM_MAINTAINER_MODE


# Check for C libraries:
AC_LANG([C++])
AC_REQUIRE_CPP

AC_LANG_PUSH([C])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(stdlib.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset select])

AC_MSG_RESULT([

		**************************************
		* Checking for C Libraries
		**************************************

	])

#WF_CHECK_SDL_IMAGE(1.2.0)
#dnl don't add SDL flags yet, SDL_main confuses later checks

#WF_CHECK_GL_SINGLE_LIB

# OpenAL check

AC_LANG_PUSH(C)

AC_CHECK_LIB(openal,
	alutInit ,,
	     AC_MSG_ERROR([Unable to find OpenAL (Open Audio Layer)])
	)

AC_CHECK_LIB([xerces-c],
	[main],,
		AC_MSG_ERROR([Cannot find xerces library. ]), 
	-lpthread)

	
	
AC_LANG_POP(C)

# Test for libxml2
#PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4.23,
#	[
#		CXXFLAGS="$CPPFLAGS $XML_CFLAGS"
#		LDFLAGS="$LDFLAGS $XML_LIBS"
#	],
# AC_MSG_ERROR(Couldn't find libxml)
#)

# Check for the FreeType 2 library
#
# Get the cflags and libraries from the freetype-config script
#AC_ARG_WITH(freetype-prefix,[  --with-freetype-prefix=PFX   Prefix where FREETYPE is 
#			       installed (optional)],
#			       freetype_prefix="$withval",
#			       freetype_prefix="")
#AC_ARG_WITH(freetype-exec-prefix,[  --with-freetype-exec-prefix=PFX Exec prefix
#				    where FREETYPE is installed (optional)],
#				    freetype_exec_prefix="$withval", freetype_exec_prefix="")
#
#if test x$freetype_exec_prefix != x ; then
#     freetype_args="$freetype_args --exec-prefix=$freetype_exec_prefix"
#     if test x${FREETYPE_CONFIG+set} != xset ; then
#        FREETYPE_CONFIG=$freetype_exec_prefix/bin/freetype-config
#     fi
#fi
#if test x$freetype_prefix != x ; then
#     freetype_args="$freetype_args --prefix=$freetype_prefix"
#     if test x${FREETYPE_CONFIG+set} != xset ; then
#        FREETYPE_CONFIG=$freetype_prefix/bin/freetype-config
#     fi
#fi
#AC_PATH_PROG(FREETYPE_CONFIG, freetype-config, no)
#no_freetype=""
#if test "$FREETYPE_CONFIG" = "no" ; then
#    AC_MSG_ERROR([
#*** Unable to find FreeType2 library (http://www.freetype.org/)
#])
#else
#    CXXFLAGS="$CXXFLAGS `$FREETYPE_CONFIG $freetypeconf_args --cflags`"
#    LIBS="$LIBS `$FREETYPE_CONFIG $freetypeconf_args --libs`"
#fi

# Check for C++ libraries:
AC_LANG_POP([C])

AC_MSG_RESULT([

		**************************************
		* Checking for C++ Libraries
		**************************************

	])

# Check for the Boost shared_ptr.hpp shared pointer header
#AC_CHECK_HEADER(boost/shared_ptr.hpp, AC_DEFINE(HAVE_LIBBOOST,1,Boost), 
#	AC_MSG_ERROR(The Boost C++ libraries are required to build Ember. They're available from www.boost.org))

#Check for the Boost bind header
AC_CHECK_HEADER(boost/bind.hpp, AC_DEFINE(HAVE_LIBBOOST,1,Boost), 
	AC_MSG_ERROR(The Boost C++ libraries are required to build Ember. They're available from www.boost.org))


AC_CHECK_HEADER(ext/hash_set, AC_DEFINE(EXT_HASH,1,"extension stl"),	)

# Check for OGRE
OGRE_VERSION=0.15.0
PKG_CHECK_MODULES(OGRE, OGRE >= 0.15.0,
	[
		CXXFLAGS="$CXXFLAGS $OGRE_CFLAGS"
		LDFLAGS="$LDFLAGS $OGRE_LIBS  `pkg-config --variable=plugindir OGRE`/Plugin_OctreeSceneManager.so"
	],
	[
		AC_MSG_ERROR(Couldn't find OGRE [or tests failed.] Please goto http://www.ogre3d.org to get the latest, or check config.log to see why the tests failed, and fix it.)
	])

# Check for CEGUI
PKG_CHECK_MODULES(CEGUI, CEGUI-OGRE >= 0.01,
	[
		CPPFLAGS="$CXXFLAGS $CEGUI_CFLAGS"
		LDFLAGS="$LDFLAGS $CEGUI_LIBS"
	],
	[
		AC_MSG_ERROR(Couldn't find CEGUI-OGRE [or tests failed]. Please go to http://crayzedsgui.sourceforge.net to get the latest, or check config.log to see why the tests failed, and fix it.)
	])



# Check for the WorldForge libs
PKG_CHECK_MODULES(WF, eris-1.3 \
 varconf-1.0 mercator-0.2,
	[
		CPPFLAGS="$CPPFLAGS $WF_CFLAGS"
		LDFLAGS="$LDFLAGS $WF_LIBS"
	],
 AC_MSG_ERROR(Couldn't find some of the required WorldForge libraries)
)
# AC_DEFINE(ATLAS_ELEMENT_TYPEDEF_PRIVATE, 1, [Remove Atlas compiler warnings])
CXXFLAGS="$CXXFLAGS -DATLAS_ELEMENT_TYPEDEF_PRIVATE"

# Cppunit check
 
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


# debugging option
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

# gprof debugging option
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


dnl disabled cal3d, until needed

#AC_ARG_WITH(cal3d, [--with-cal3d=prefix 	Specify where Cal3d is installed too], [
#	CPPFLAGS="$CPPFLAGS -I$withval/include"
#	LIBS="$LIBS -L$withval/lib"
#])

#AC_CHECK_HEADER(cal3d/cal3d.h,,AC_MSG_ERROR(Couldn't find the Cal3d headers))

# verify that we can link against Cal3d, but don't add it to LIBS
#saved_LIBS=$LIBS
#AC_CHECK_LIB(cal3d, main, , AC_MSG_ERROR(Couldn't find the Cal3d library))
#LIBS=$saved_LIBS


#AC_LANG([C++])

dnl add SDL flags here, now that we're done with the other tests
CXXFLAGS="$CXXFLAGS $WF_SDL_IMAGE_CFLAGS"
LIBS="$LIBS $WF_SDL_IMAGE_LIBS"

AC_MSG_RESULT([

		**************************************
		* Tests Passed, Creating Makefiles.
		**************************************

	])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/components/Makefile
		 src/framework/Makefile
		 src/services/Makefile
		 src/services/config/Makefile
		 src/services/logging/Makefile
		 src/services/metaserver/Makefile
		 src/services/server/Makefile
		 src/services/sound/Makefile
		 ember.dox
		 ember.spec
		 src/components/ogre/Makefile
])

#		 src/tests/Makefile
#		 src/components/text/Makefile
#		 src/services/input/Makefile
#		 src/services/image/Makefile
#		 src/services/font/Makefile
#		 src/services/gui/Makefile
#		 src/services/gui/widget/Makefile
#		 src/services/platform/Makefile
#		 src/components/debug/Makefile
#		 src/services/test/Makefile

AC_OUTPUT

AC_MSG_RESULT([

		**************************************
		* Finished.  Run make to compile Ember.
		**************************************

	])

