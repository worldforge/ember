## Process this file with autoconf to produce a configure script.
AC_INIT(Ember, 0.5.7, clients@worldforge.org)

AC_CONFIG_SRCDIR(src)

# Detect the canonical host and target build environment.
AC_CANONICAL_SYSTEM

#AM_INIT_AUTOMAKE([tar-ustar nostdinc dist-bzip2 check-news])
AM_INIT_AUTOMAKE([tar-ustar nostdinc dist-bzip2])
AM_CONFIG_HEADER(src/config.h)

AC_LIBTOOL_DLOPEN

AC_PREREQ(2.50)
AC_COPYRIGHT([This script is under the GPL and is owned by the people in AUTHORS])

AC_DISABLE_SHARED

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

AC_PATH_PROG([XMLLINT], [xmllint], [true])


# Check for C libraries:
AC_LANG([C++])
AC_REQUIRE_CPP

AC_LANG_PUSH([C])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(math.h)
dnl Test for <math.h> header file
AC_CHECK_HEADERS([signal.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_STRUCT_TM

# check sighandler_t in C++
AC_LANG_PUSH([C++])
AC_CHECK_TYPES([sighandler_t],[],[],[
#ifdef HAVE_SIGNAL_H
# include <signal.h>
#endif])
AC_LANG_POP([C++])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset select])

#AC_CHECK_FUNC(lrintf,[
#  AC_DEFINE(HAVE_LRINTF, 1, [whether lrintf() is available])
#])

#AC_CHECK_FUNC(lrint,[
#  AC_DEFINE(HAVE_LRINT, 1, [whether lrint() is available])
#])

#taken from Sear
STATIC_LIBSTDCPP=
STATIC_LIBGCC=

AC_ARG_ENABLE(static_build,
  [AS_HELP_STRING([--enable-static-build],[enable static link of some libs (default=no)])],
  [
    if test "$enableval" = "yes"; then
      link_static="true"
      STATIC_LIBSTDCPP=`$CXX -print-file-name=libstdc++.a`
      STATIC_LIBGCC=`$CXX -print-file-name=libgcc.a`
    else
      link_static="false"
    fi
  ],[
    link_static="false"
  ]
)
AM_CONDITIONAL(BUILD_STATIC, test x"$link_static" = xtrue)

AC_ARG_ENABLE(freeimage_check,
  [  AS_HELP_STRING([--disable-freeimage-check], [disable the check for if Ogre is built with FreeImage. Disable this only if you're absolutly sure that libraries in FreeImage won't clash with other system libraries (default is enabled)])],
  [
    if test "$enableval" = "yes"; then
      freeimage_check="true"
    else
      freeimage_check="false"
    fi
  ],[
    freeimage_check="true"
  ]
)

AC_MSG_RESULT([

		**************************************
		* Checking for SDL Libraries
		**************************************

	])
	
AM_PATH_SDL(1.2.0, 
	    ,
[
  AC_MSG_ERROR(Couldn't find SDL or insufficent version found. Please goto http://www.libsdl.org/ and get at least version $SDL_VERSION)
])

#dnl don't add SDL flags yet, SDL_main confuses later checks

WF_CHECK_GL_SINGLE_LIB

# OpenAL check
AC_MSG_RESULT([

		**************************************
		* Checking for C Libraries
		**************************************

	])

AC_LANG_PUSH(C)

#disable this for now
#AC_CHECK_LIB(openal,
#	alutInit ,,
#	     AC_MSG_ERROR([Unable to find OpenAL (Open Audio Layer)])
#	)



	
	
AC_LANG_POP(C)

# make sure binreloc funtionality is used
AM_BINRELOC

# Try to find system-wide installation of tinyxml
# If found, add define to config.h, set LIBS and add automake conditional
AC_CHECK_LIB(tinyxml, main,
	[
		AC_DEFINE(HAVE_LIBTINYXML, 1, [Define to 1 if you have the `tinyxml' library (-ltinyxml).])
		LIBS="-ltinyxml $LIBS"
		have_tinyxml=true
	],
	[
		have_tinyxml=false
	]
)
AM_CONDITIONAL(HAVE_LIBTINYXML, [test x$have_tinyxml = xtrue])

# Check for C++ libraries:
AC_LANG_POP([C])

AC_MSG_RESULT([

		**************************************
		* Checking for C++ Libraries
		**************************************

	])
#AC_CHECK_LIB(IL, ilInit,
#	[
#		CXXFLAGS="$CXXFLAGS $IL_CFLAGS"
#		LIBS="$LIBS $IL_LIBS -lIL"
#	]
#,AC_MSG_ERROR([
#****************************************************************
#* You do not have DevIL installed.  This is required to build. *
#* You may find it at http://openil.sourceforge.net/.           *
#****************************************************************]))
#AC_CHECK_LIB(ILU, iluFlipImage,
#	[
#		CXXFLAGS="$CXXFLAGS $ILU_CFLAGS"
#		LIBS="$LIBS $ILU_LIBS -lILU"
#	]
#,AC_MSG_ERROR([
#****************************************************************
#* You do not have DevIL installed.  This is required to build. *
#* You may find it at http://openil.sourceforge.net/.           *
#****************************************************************]))

#AC_DEFINE(LIBSIGC_DISABLE_DEPRECATED, 1, [Don't use deprecated sigc methods.])


AC_CHECK_HEADER(ext/hash_set, AC_DEFINE(EXT_HASH,1,"extension stl"),	)

PKG_CHECK_MODULES(CEGUI, CEGUI >= 0.6.2,
	[
		CXXFLAGS="$CXXFLAGS $CEGUI_CFLAGS"
		LIBS="$LIBS $CEGUI_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find CEGUI-OGRE or tests failed:
$CEGUI_PKG_ERRORS
Please go to http://crayzedsgui.sourceforge.net to get the latest, or check
config.log to see why the tests failed, and fix it.])
	])

# Check for OGRE
OGRE_VERSION=1.6.0
OGRE_MAX_VERSION=1.8.0
PKG_CHECK_MODULES(OGRE, [OGRE >= $OGRE_VERSION OGRE < $OGRE_MAX_VERSION CEGUI-OGRE >= $OGRE_VERSION],
	[
		CXXFLAGS="$CXXFLAGS $OGRE_CFLAGS"
		LIBS="$LIBS $OGRE_LIBS"
		AC_DEFINE_UNQUOTED([OGRE_PLUGINDIR], "`pkg-config --variable=plugindir OGRE`", [The Ogre plugin dir])
	],
	[
		AC_MSG_ERROR([Couldn't find OGRE or tests failed:
$OGRE_PKG_ERRORS
Please goto http://www.ogre3d.org to get the latest, or check config.log to
see why the tests failed, and fix it.])
	])

if test "$freeimage_check" = "true"; then 
	AC_CHECK_DECLS(OGRE_NO_FREEIMAGE
	, , [AC_MSG_NOTICE([You've compiled Ogre with FreeImage support. Due to the way old versions of FreeImage includes its own versions of common image libraries such as libpng this could clash with other system libs such as DevIL, leading to random crashes. If you have old version of FreeImage, please compile Ogre with FreeImage disabled (by using the --disable-freeimage configure switch). Ogre will then use DevIL instead.])]
	)
else
	AC_MSG_RESULT(
	[********
You've chosen to disable the FreeImage tests. Proceed on your own risk.
********])
fi



#check for lua
PKG_CHECK_MODULES(LUA, lua5.1 >= 5.1,
[
	CXXFLAGS="$CXXFLAGS $LUA_CFLAGS -DLUA51"
	LIBS="$LIBS $LUA_LIBS"
	AC_MSG_NOTICE([Lua version => 5.1, make sure that CEGUI also has been compiled for this version])
],
[
	PKG_CHECK_MODULES(LUA, lua >= 5.1,
	[
		CXXFLAGS="$CXXFLAGS $LUA_CFLAGS -DLUA51"
		LIBS="$LIBS $LUA_LIBS"
		AC_MSG_NOTICE([Lua version => 5.1, make sure that CEGUI also has been compiled for this version])
	],
	[
		AC_MSG_ERROR([Couldn't find Lua or tests failed. Error: $LUA_PKG_ERRORS. Please go to http://www.lua.org to get the latest, or check config.log to see why the tests failed, and fix it. Also make sure that CEGUI is built with Lua support.])
	])
])

# Check for tolua++
AC_CHECK_LIB([CEGUItoluapp], [main],
    [
        LIBS="-lCEGUItoluapp $LIBS"
    ],
    [
        AC_CHECK_LIB([tolua++], main,
            [
                LIBS="-ltolua++ $LIBS"
            ],
            [
                AC_MSG_ERROR([Couldn't find tolua++. Make sure that CEGUI compiled with Lua scripting support.])
            ]
        )
    ]
)

# We have lua and tolua++. Now, check for CEGUI lua module
AC_CHECK_LIB([CEGUILuaScriptModule], [main],
    [
        LIBS="-lCEGUILuaScriptModule $LIBS"
    ],
    [
        AC_MSG_ERROR([Couldn't find Lua CEGUI module. Make sure that CEGUI compiled with Lua scripting support.])
    ]
)



# Check for OpenAL
OPENAL_VERSION=0.0.8
PKG_CHECK_MODULES(OPENAL, openal >= $OPENAL_VERSION,
	[
		CXXFLAGS="$CXXFLAGS $OPENAL_CFLAGS"
		LIBS="$LIBS $OPENAL_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find OpenAL or tests failed. Error: $OPENAL_PKG_ERRORS. Please go to http://www.openal.org to get the latest, or check config.log to see why the tests failed, and fix it.])
	])

#check for libogg 
#OGG_VERSION=1.1.3
#PKG_CHECK_MODULES(OGG, ogg >= $OGG_VERSION, 
#		[
#			CXXFLAGS="$CXXFLAGS $OGG_CFLAGS"
#			LIBS="$LIBS $OGG_LIBS"
#		],
#		[
#			AC_MSG_ERROR([Couldn't find libogg or tests failed. Error: $OGG_PKG_ERRORS. Please go to http://www.vorbis.com to get the latest, or check config.log to see why the tests failed, and fix it.])
#		])

#check for vorbis 
#VORBIS_VERSION=1.2.0
#PKG_CHECK_MODULES(VORBIS, vorbis >= $VORBIS_VERSION, 
#		[
#			CXXFLAGS="$CXXFLAGS $VORBIS_CFLAGS"
#			LIBS="$LIBS $VORBIS_LIBS"
#		],
#		[
#			AC_MSG_ERROR([Couldn't find libvorbis or tests failed. Error: $VORBIS_PKG_ERRORS. Please go to http://www.vorbis.com to get the latest, or check config.log to see why the tests failed, and fix it.])
#		])

#check for vorbisfile 
#VORBISF_VERSION=1.2.0
#PKG_CHECK_MODULES(VORBISF, vorbisfile >= $VORBISF_VERSION, 
#		[
#			CXXFLAGS="$CXXFLAGS $VORBISF_CFLAGS"
#			LIBS="$LIBS $VORBISF_LIBS"
#		],
#		[
#			AC_MSG_ERROR([Couldn't find vorbisfile tests failed. Error: $VORBISF_PKG_ERRORS. Please go to http://www.vorbis.com to get the latest, or check config.log to see why the tests failed, and fix it.])
#		])

# Check for FreeALUT
FREEALUT_VERSION=1.0.0
PKG_CHECK_MODULES(FREEALUT, freealut >= $FREEALUT_VERSION,
	[
		CXXFLAGS="$CXXFLAGS $FREEALUT_CFLAGS"
		LIBS="$LIBS $FREEALUT_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find FreeALUT or tests failed. Error: $FREEALUT_PKG_ERRORS. Please go to http://www.openal.org to get the latest, or check config.log to see why the tests failed, and fix it.])
	])



#CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"

#SDL_VERSION=1.2.0
#AC_SUBST(SDL_VERSION)

# check for sigc++ separately from the WF checks
PKG_CHECK_MODULES(SIG, [sigc++-2.0 >= 2.0],
  [
      CXXFLAGS="$CXXFLAGS $SIG_CFLAGS"
      LIBS="$LIBS $SIG_LIBS"
  ],
  [
      AC_MSG_ERROR([Couldn't find SIGC++2.0])
  ])

# Check for Boost::Thread
AC_CHECK_LIB([boost_thread-mt], [main],
    [
        LIBS="$LIBS -lboost_thread-mt"
    ],
    [
        AC_MSG_ERROR([Couldn't find Boost::Thread library.])
    ]
)


# Check for the WorldForge libs
PKG_CHECK_MODULES(WF, [eris-1.3 >= 1.3.14 \
 varconf-1.0 >= 0.6.5 mercator-0.2 >= 0.2.6 atlascpp-0.6 >= 0.6.0 wfmath-0.3 >= 0.3.9 libwfut-0.2 >= 0.2.0],
	[
		CXXFLAGS="$CXXFLAGS $WF_CFLAGS"
		LIBS="$LIBS $WF_LIBS"
	],
 AC_MSG_ERROR([Couldn't find some of the required WorldForge libraries: $WF_PKG_ERRORS
Please visit  http://worldforge.org/dev/eng/libraries to get the latest versions.])
)



# Cppunit check
 
#AC_ARG_ENABLE(cppunit,[  --enable-cppunit     enables cppunit tests [default=yes]],
#[ 
#   if test x$enableval = xno; then 
#		ac_use_cppunit="no"	
#   else 
#		ac_use_cppunit="yes"
#   fi
#], [ac_use_cppunit="yes"])

#found_cppunit=no

#if test "$ac_use_cppunit" = "yes"; then
#AM_PATH_CPPUNIT(1.8.0, [
#  AC_MSG_RESULT([
#*** Found CppUnit
#*** Setting up make check to run tests...])
#	CXXFLAGS="$CXXFLAGS -DUSE_CPP_UNIT"
#	LIBS="$LIBS $CPPUNIT_LIBS"
#], 
#  AC_MSG_RESULT([
#
#*** Unable to find a recent enough CppUnit
#*** Will not run unit testing!!
#*** 
#*** To get CppUnit see http://cppunit.sourceforge.net
#]))
#fi


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
      CFLAGS="$CFLAGS -pg -g"
      CXXFLAGS="$CXXFLAGS -pg -g"
fi






#dnl add SDL flags here, now that we're done with the other tests
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"
    

AC_MSG_RESULT([

		**************************************
		* Tests Passed, Creating Makefiles.
		**************************************

	])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/main/Makefile
		 src/main/bindings/Makefile
		 src/main/bindings/lua/Makefile
		 src/components/Makefile
		 src/framework/Makefile
		 src/framework/bindings/Makefile
		 src/framework/bindings/lua/Makefile
		 src/framework/bindings/lua/eris/Makefile
		 src/framework/bindings/lua/atlas/Makefile
		 src/framework/bindings/lua/varconf/Makefile
		 src/services/Makefile
		 src/services/config/Makefile
		 src/services/logging/Makefile
		 src/services/metaserver/Makefile
		 src/services/server/Makefile
		 src/services/scripting/Makefile
		 src/services/sound/Makefile
		 src/services/time/Makefile
		 src/services/bindings/Makefile
		 src/services/bindings/lua/Makefile
		 src/services/wfut/Makefile
		 src/services/input/Makefile
		 ember.dox
		 ember
		 ember.spec
		 ember-media.spec
		 autopackage/default.apspec
		 src/extensions/Makefile
		 src/components/ogre/scripting/Makefile
		 src/components/ogre/scripting/bindings/Makefile
		 src/components/ogre/scripting/bindings/lua/Makefile
		 src/components/ogre/scripting/bindings/lua/ogre/Makefile
		 src/components/ogre/scripting/bindings/lua/helpers/Makefile
		 src/components/ogre/sound/Makefile
		 src/components/ogre/ogreopcode/Makefile
		 src/components/ogre/ogreopcode/src/Makefile
		 src/components/ogre/ogreopcode/src/Opcode/Makefile
		 src/components/ogre/ogreopcode/include/Makefile
		 src/components/ogre/ogreopcode/include/Opcode/Makefile
		 src/components/ogre/Makefile
		 src/components/ogre/gui/Makefile
		 src/components/ogre/cegui/Makefile
		 src/components/ogre/widgets/Makefile
		 src/components/ogre/widgets/adapters/Makefile
		 src/components/ogre/widgets/adapters/atlas/Makefile
		 src/components/ogre/widgets/icons/Makefile
		 src/components/ogre/environment/meshtree/Makefile
		 src/components/ogre/environment/caelum/Makefile
		 src/components/ogre/environment/hydrax/Makefile
		 src/components/ogre/environment/pagedgeometry/Makefile
		 src/components/ogre/environment/Makefile
		 src/components/ogre/model/Makefile
		 src/components/ogre/mapping/Makefile
		 src/components/ogre/modeldefinitions/Makefile
		 src/components/ogre/sounddefinitions/Makefile
		 src/components/ogre/carpenter/Makefile
		 src/components/ogre/jesus/Makefile
		 src/components/ogre/manipulation/Makefile
		 src/components/ogre/manipulation/entityrecipes/Makefile
		 src/components/ogre/terrain/Makefile
		 src/components/ogre/SceneManagers/Makefile
		 src/components/ogre/SceneManagers/EmberPagingSceneManager/Makefile
		 src/components/ogre/SceneManagers/EmberPagingSceneManager/src/Makefile
		 src/components/ogre/SceneManagers/EmberPagingSceneManager/include/Makefile
		 src/components/entitymapping/Makefile
		 media/Makefile
		 media/packs/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([

		**************************************
		* Finished.  Run make to compile Ember.
		* Note that you also need to install the media. This can be gotten in a couple of different ways.
		* The easiest way is to use the rsync make target (which uses the rsync program to update the media.).
		* If you're compiling from git and want to use the development version, do:
		*
		* make devmedia
		*
		* If you're using a release version, you should however instead do:
		*
		* make releasemediarsync
		*
		* Note that both of these make targets will just fetch and install the files directly, bypassing normal "make install" producures. They should therefore not be used if you don't want to pollute your installation directory, and shouldn't be used if you install to a system directory.
		*
		* A third way is to get the media tarball directly. This only works for release versions however.
		* You can find these media tarballs at http://amber.worldforge.org/WFUT .
		* The media should be installed to PREFIX/share/ember/media
		* so that you get PREFIX/share/ember/media/user and PREFIX/share/ember/media/shared.
		* More information can be found at the Ember page at http://www.worldforge.org
		**************************************

	])

