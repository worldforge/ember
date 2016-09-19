## Process this file with autoconf to produce a configure script.
AC_INIT([Ember],[0.8.0],[erik@ogenvik.org])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR(src)

# Detect the canonical host and target build environment.
AC_CANONICAL_TARGET

# We have to use "foreign" since we supply README.md instead of the GNU canonical README file.
AM_INIT_AUTOMAKE([tar-ustar nostdinc dist-bzip2 color-tests parallel-tests foreign subdir-objects])
AM_SILENT_RULES([yes])


AC_CONFIG_HEADERS([src/config.h])

AC_PREREQ([2.50])
AC_COPYRIGHT([This script is under the GPL and is owned by the people in AUTHORS])

#will disable verbose linking checks, which fail even if there is no problem.
lt_cv_deplibs_check_method=pass_all
LT_INIT


# Checks for Programs
AC_PROG_CXX
AC_PROG_CC
AC_PROG_OBJC
AC_PROG_LIBTOOL

AC_PATH_PROG([XMLLINT], [xmllint], [true])

# Check for C libraries:
AC_LANG([C++])
AC_REQUIRE_CPP
AX_CXX_COMPILE_STDCXX_11


AC_LANG_PUSH([C])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER(stdlib.h)
AC_CHECK_HEADER(math.h)
AC_CHECK_HEADER([signal.h])

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
AM_CONDITIONAL([BUILD_STATIC], [test x"$link_static" = xtrue])

AC_MSG_RESULT([

		**************************************
		* Checking for SDL Libraries
		**************************************

	])
	

SDL_VERSION="2.0.0"
AM_PATH_SDL2([$SDL_VERSION],,
[
  AC_MSG_ERROR(Couldn't find SDL or insufficent version found. Please goto http://www.libsdl.org/ and get at least version $SDL_VERSION)
])

#dnl don't add SDL flags yet, SDL_main confuses later checks

#We check for OpenGL only because we want to check whether indirect rendering is used or not. If the code is altered to not having to check with OpenGL direct (for example going through Ogre) this check can be removed. 
WF_CHECK_GL_SINGLE_LIB

# OpenAL check
AC_MSG_RESULT([

		**************************************
		* Checking for C Libraries
		**************************************

	])

WIN32="false"
OSX="false"
case $build_os in
  cygwin* | mingw* | cegcc*)
	LIBS="$LIBS -lshlwapi"
	AC_CHECK_TOOL(WINDRES, windres, "windres")
	AC_MSG_RESULT([Building for windows.])
	WIN32="true"
    ;;
  darwin*)
    AC_MSG_RESULT([Building for Mac OS X.])
	OSX="true"
    ;;
esac

AM_CONDITIONAL([WIN32], [test x$WIN32 = xtrue])
AM_CONDITIONAL([OSX], [test x$OSX = xtrue])

BOOST_REQUIRE
BOOST_THREADS
BOOST_DATE_TIME
BOOST_SYSTEM

LDFLAGS="$LDFLAGS $BOOST_THREAD_LDFLAGS $BOOST_DATE_TIME_LDFLAGS $BOOST_SYSTEM_LDFLAGS"
LIBS="$LIBS $BOOST_THREAD_LIBS $BOOST_DATE_TIME_LIBS $BOOST_SYSTEM_LIBS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"

AC_ARG_ENABLE(webember,
  [AS_HELP_STRING([--enable-webember], [enable building of webember (default=no)])],
  [
    if test "$enableval" = "yes"; then
      webember_enabled="true"
      CXXFLAGS="$CXXFLAGS -DBUILD_WEBEMBER"
    else
      webember_enabled="false"
    fi
  ],[
    webember_enabled="false"
  ]
)

AM_CONDITIONAL([BUILD_WEBEMBER], [test x"$webember_enabled" = xtrue])

if test "$webember_enabled" = "true" -a "$WIN32" = "false" -a "$OSX" = "false"  ; then
  # needed for some autotools versions. This is needed on POSIX ELF shared libraries only.
  CXXFLAGS="$CXXFLAGS -fPIC"
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([boost/interprocess/ipc/message_queue.hpp], [],
 		[AC_MSG_ERROR(You need the Boost interprocess library.)])
	AC_LANG_POP([C++])
fi



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


# Check for the WorldForge libs
PKG_CHECK_MODULES(WF, [eris-1.4 >= 1.4.0 \
 varconf-1.0 >= 0.6.7 mercator-0.4 >= 0.4.0 atlascpp-0.7 >= 0.7.0 wfmath-1.0 >= 1.0.3 libwfut-0.2 >= 0.2.2],
	[
		CXXFLAGS="$CXXFLAGS $WF_CFLAGS"
		LIBS="$LIBS $WF_LIBS"
	],
 AC_MSG_ERROR([Couldn't find some of the required WorldForge libraries: $WF_PKG_ERRORS
Please visit  http://worldforge.org/dev/eng/libraries to get the latest versions.])
)

# Check for OGRE
OGRE_VERSION=1.9.0
OGRE_MAX_VERSION=1.9.9
PKG_CHECK_MODULES(OGRE, [OGRE >= $OGRE_VERSION OGRE < $OGRE_MAX_VERSION],
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

# Check for OGRE Overlay Component separately
OGRE_OVERLAY_VERSION=1.9.0
PKG_CHECK_MODULES(OGRE_OVERLAY, [OGRE-Overlay >= $OGRE_OVERLAY_VERSION],
    [
        CXXFLAGS="$CXXFLAGS $OGRE_OVERLAY_CFLAGS"
        LIBS="$LIBS $OGRE_OVERLAY_LIBS"
    ],
		AC_MSG_ERROR([Could not find the required OGRE Overlay component:
$OGRE_OVERLAY_PKG_ERRORS
Please goto http://www.ogre3d.org to get the full OGRE libraries, or check config.log to
see why the tests failed, and fix it.])
                  )
AC_SUBST([OGRE_OVERLAY_CFLAGS])
AC_SUBST([OGRE_OVERLAY_LIBS])

# Check for OGRE Terrain Component separately
OGRE_TERRAIN_VERSION=1.9.0
PKG_CHECK_MODULES(OGRE_TERRAIN, [OGRE-Terrain >= $OGRE_TERRAIN_VERSION],
    [
        CXXFLAGS="$CXXFLAGS $OGRE_TERRAIN_CFLAGS"
        LIBS="$LIBS $OGRE_TERRAIN_LIBS"                                         
    ],
		AC_MSG_ERROR([Could not find the required OGRE Terrain component:
$OGRE_TERRAIN_PKG_ERRORS
Please goto http://www.ogre3d.org to get the full OGRE libraries, or check config.log to
see why the tests failed, and fix it.])
                  )
AC_SUBST([OGRE_TERRAIN_CFLAGS])
AC_SUBST([OGRE_TERRAIN_LIBS])

	
#Check that Ogre is built with FreeImage support. If not, no images can be loaded. Ogre can also be built with DevIL, but that's not thread safe, and will lead to crashes.
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <OgreBuildSettings.h>
]],
[[
#if OGRE_NO_FREEIMAGE == 0
// OGRE is built with FreeImage
#else
# error OGRE is not built with FreeImage
#endif
]])],
[], [AC_MSG_ERROR([[You must build Ogre with FreeImage support. You cannot use DevIL, as that's not thread safe.]])])

# We need to check if Ogre is built in double precision mode, mainly in order for the Lua bindings to be correct.
REAL_TYPE=float
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <OgreBuildSettings.h>
]],
[[
#if OGRE_DOUBLE_PRECISION == 0
// OGRE is built with single precision
#else
# error OGRE is built with double precision
#endif
]])],
[],[REAL_TYPE="double"])

AC_SUBST(OGRE_REAL_TYPE, $REAL_TYPE)

CEGUI_VERSION=0.8.3
CEGUI_PKG=CEGUI-0
PKG_CHECK_MODULES(CEGUI, $CEGUI_PKG >= $CEGUI_VERSION,
	[
		CXXFLAGS="$CXXFLAGS $CEGUI_CFLAGS"
		LIBS="$LIBS $CEGUI_LIBS"
		CEGUI_PREFIX=`pkg-config $CEGUI_PKG --variable=prefix`
		CEGUI_MODULEDIR=`pkg-config $CEGUI_PKG --variable=moduledir`
		dnl Try to figure out the relative path of the module dir, to use in ember.in. 
		dnl This of course won't work if the prefix isn't part of the module dir path. 
		CEGUI_MODULE_SUBDIR=${CEGUI_MODULEDIR#$CEGUI_PREFIX}
		AC_SUBST(CEGUI_MODULEDIR)
		AC_SUBST(CEGUI_MODULE_SUBDIR)

		#Check that CEGUI is built with FreeType support. This seems to be a common error.
		AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
			#include <CEGUI/Config.h>
			]],
			[[
			#ifdef CEGUI_HAS_FREETYPE
			// CEGUI is built with Freetype
			#else
			# CEGUI is not built with Freetype
			#endif
			]])],
			[], [AC_MSG_ERROR([[You must build CEGUI with Freetype support.]])])	

	],
	[
		AC_MSG_ERROR([Couldn't find CEGUI or tests failed:
$CEGUI_PKG_ERRORS
Please go to http://www.cegui.org.uk to get the latest, or check
config.log to see why the tests failed, and fix it.])
	])
	
case "x$target_os" in
  *bsd* | *linux* )
	PKG_CHECK_MODULES([X11], x11,
		[
			CXXFLAGS="$CXXFLAGS $X11_CFLAGS"
			LIBS="$LIBS $X11_LIBS"
		],
		[
			AC_MSG_ERROR([Couldn't find X11 development files.
			$X11_PKG_ERRORS])
		])
  break;
  ;;
esac
	

# Check for CEGUI-OGRE adapter
PKG_CHECK_MODULES(CEGUIOGRE, [CEGUI-0-OGRE >= $CEGUI_VERSION],
	[
		CXXFLAGS="$CXXFLAGS $CEGUIOGRE_CFLAGS"
		LIBS="$LIBS $CEGUIOGRE_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find CEGUI-OGRE adapter. Make sure that you've compiled Ogre before compiling CEGUI, and that CEGUI found the Ogre libs so that the Ogre renderer is built.
$CEGUIOGRE_PKG_ERRORS])
	])


#check for lua 5.1. On Debian system the pkg-config file is "lua5.1.pc", on Fedora it's "lua-5.1.pc" and on other system it's just "lua.pc".
PKG_CHECK_MODULES(LUA, lua5.1 >= 5.1,
[
	CXXFLAGS="$CXXFLAGS $LUA_CFLAGS -DLUA51"
	LIBS="$LIBS $LUA_LIBS"
	AC_MSG_NOTICE([Lua version = 5.1, make sure that CEGUI also has been compiled for this version])
],
[
	PKG_CHECK_MODULES(LUA, lua-5.1 >= 5.1,
	[
		CXXFLAGS="$CXXFLAGS $LUA_CFLAGS -DLUA51"
		LIBS="$LIBS $LUA_LIBS"
		AC_MSG_NOTICE([Lua version = 5.1, make sure that CEGUI also has been compiled for this version])
	],
	[
		PKG_CHECK_MODULES(LUA, lua >= 5.1 lua < 5.2,
		[
			CXXFLAGS="$CXXFLAGS $LUA_CFLAGS -DLUA51"
			LIBS="$LIBS $LUA_LIBS"
			AC_MSG_NOTICE([Lua version = 5.1, make sure that CEGUI also has been compiled for this version])
		],
		[
			AC_MSG_ERROR([Couldn't find Lua 5.1 or tests failed. Error: $LUA_PKG_ERRORS. Please go to http://www.lua.org to get the latest version of Lua 5.1, or check config.log to see why the tests failed, and fix it. Also make sure that CEGUI is built with Lua support.])
		])
	])
])


AC_ARG_ENABLE(cegui_toluapp,
  [  AS_HELP_STRING([--enable-cegui-toluapp], [allow Ember to use the CEGUI provided tolua++ library. This requires that you've also had CEGUI build the tolua++ generator, and that generator by this build (see --with-tolua++) (default is disabled)])],
  [
    if test "$enableval" = "yes"; then
      cegui_toluapp="true"
    else
      cegui_toluapp="false"
    fi
  ],[
    cegui_toluapp="false"
  ]
)

# Check for tolua++
AC_CHECK_LIB([CEGUItoluapp], [main],
    [
	    if test "$cegui_toluapp" = "true"; then
	        LIBS="-lCEGUItoluapp $LIBS"
	        AC_MSG_NOTICE([Using CEGUI provided tolua++ implementation. Make sure that you're also using the CEGUI provided tolua++ generator (see --with-toluaxx)])
	    else
	    	AC_MSG_ERROR([A CEGUI provided tolua++ implementation was found. By default this is not allowed. The reason is that CEGUI normally doesn't build the tolua++ generator needed by Ember, and there's a risk then that Ember will use a tolua++ generator installed by the system together with the tolua++ library provided by CEGUI. This will in most cases lead to strange compilation errors.
	    		You have two options:
	    			1) Make sure that there's a tolua++ (http://www.codenix.com/~tolua/) development package installed on the system, and recompile CEGUI so that it recognizes and uses this (make sure to uninstall CEGUI first).
	    			2) If you're certain that you've told CEGUI to also compile the tolua++ code generator and want to have Ember use the CEGUI provided tolua++ implementation, use the --enable-cegui-toluapp to disable this check. Use --with-toluaxx to make sure that the CEGUI provided tolua++ generator is used.])
	    fi
    ],
    [
    	PKG_CHECK_MODULES(TOLUAPP, tolua++,
		[
			CXXFLAGS="$CXXFLAGS $TOLUAPP_CFLAGS"
			LIBS="$LIBS $TOLUAPP_LIBS"
		],
		[
	        AC_CHECK_LIB([tolua++], [main],
		        [
		            LIBS="-ltolua++ $LIBS"
		        ],
		        [
		        	#Debian based systems adds a "5.1" suffix to the library name
			        AC_CHECK_LIB([tolua++5.1], [main],
			            [
			                LIBS="-ltolua++5.1 $LIBS"
			            ],
			            [
			            	#Yet another version is to name the library "toluapp".
			                AC_CHECK_LIB([toluapp], [main],
					            [
					                LIBS="-ltoluapp $LIBS"
					            ],
					            [
					                AC_MSG_ERROR([Couldn't find tolua++. Make sure that tolua++ (http://www.codenix.com/~tolua/) is installed and CEGUI is compiled with Lua scripting support.])
					            ]
					        )
			            ]
			        )
		        ]
		    )		
		])
    ]
)

AM_CHECK_TOLUAXX

# We have lua and tolua++. Now, check for CEGUI lua module
PKG_CHECK_MODULES(CEGUILUA, [CEGUI-0-LUA >= $CEGUI_VERSION],
	[
		CXXFLAGS="$CXXFLAGS $CEGUILUA_CFLAGS"
		LIBS="$LIBS $CEGUILUA_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find Lua CEGUI module. Make sure that CEGUI compiled with Lua scripting support.
$CEGUILUA_PKG_ERRORS])
	])


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

# Check for libxdg-basedir
if test "x$WIN32" = "xfalse" -a "x$OSX" = "xfalse"; then
XDGBASEDIR_VERSION=1.2.0
PKG_CHECK_MODULES(XDGBASEDIR, libxdg-basedir >= $XDGBASEDIR_VERSION,
	[
		CXXFLAGS="$CXXFLAGS $XDGBASEDIR_CFLAGS"
		LIBS="$LIBS $XDGBASEDIR_LIBS"
	],
	[
		AC_MSG_ERROR([Couldn't find libXDG-basedir or tests failed. Error: $XDGBASEDIR_PKG_ERRORS. Please go to http://nevill.ch/libxdg-basedir/ to get the latest, or check config.log to see why the tests failed, and fix it.])
	])
fi

# check for sigc++ separately from the WF checks
PKG_CHECK_MODULES(SIG, [sigc++-2.0 >= 2.0],
  [
      CXXFLAGS="$CXXFLAGS $SIG_CFLAGS"
      LIBS="$LIBS $SIG_LIBS"
  ],
  [
      AC_MSG_ERROR([Couldn't find SIGC++2.0])
  ])


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
			found_cppunit=yes
		], 
	  AC_MSG_RESULT([
		*** Unable to find a recent enough CppUnit
		*** Will not run unit testing!!
		*** 
		*** To get CppUnit see http://cppunit.sourceforge.net
		]))
fi

AM_CONDITIONAL(USE_CPPUNIT, [test x$found_cppunit = xyes])


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
		 src/platform/Makefile
		 src/main/Makefile
		 src/components/Makefile
		 src/bindings/Makefile
		 src/bindings/lua/Makefile
		 src/framework/Makefile
		 src/framework/tasks/Makefile
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
		 src/services/bindings/Makefile
		 src/services/bindings/lua/Makefile
		 src/services/wfut/Makefile
		 src/services/input/Makefile
		 src/services/serversettings/Makefile
		 ember.dox
		 ember
		 ember.conf
		 support/ember.spec
		 support/ember-media.spec
		 src/extensions/Makefile
		 src/components/lua/Makefile
		 src/components/lua/bindings/Makefile
		 src/components/lua/bindings/lua/Makefile
		 src/components/ogre/scripting/Makefile
		 src/components/ogre/scripting/bindings/Makefile
		 src/components/ogre/scripting/bindings/lua/Makefile
		 src/components/ogre/scripting/bindings/lua/ogre/Makefile
		 src/components/ogre/scripting/bindings/lua/helpers/Makefile
		 src/components/ogre/Makefile
		 src/components/ogre/widgets/Makefile
		 src/components/ogre/widgets/adapters/Makefile
		 src/components/ogre/widgets/adapters/atlas/Makefile
		 src/components/ogre/widgets/adapters/atlas/bindings/Makefile
		 src/components/ogre/widgets/adapters/atlas/bindings/lua/Makefile
		 src/components/ogre/widgets/adapters/bindings/Makefile
		 src/components/ogre/widgets/adapters/bindings/lua/Makefile
		 src/components/ogre/widgets/representations/Makefile
		 src/components/ogre/widgets/representations/bindings/Makefile
		 src/components/ogre/widgets/representations/bindings/lua/Makefile
		 src/components/ogre/environment/caelum/Makefile
		 src/components/ogre/environment/pagedgeometry/Makefile
		 src/components/ogre/environment/meshtree/Makefile
		 src/components/ogre/environment/Makefile
		 src/components/ogre/sounddefinitions/Makefile
		 src/components/ogre/authoring/Makefile
		 src/components/ogre/authoring/entityrecipes/Makefile
		 src/components/entitymapping/Makefile
		 src/components/terrain/Makefile
		 src/components/cegui/Makefile
		 src/components/cegui/bindings/Makefile
		 src/components/cegui/bindings/lua/Makefile
		 src/components/navigation/Makefile
		 src/components/navigation/external/Makefile
		 src/components/navigation/external/RecastDetour/Makefile
		 src/components/navigation/external/RecastDetour/Recast/Makefile
		 src/components/navigation/external/RecastDetour/Detour/Makefile
		 src/components/navigation/external/RecastDetour/DetourTileCache/Makefile
		 test/Makefile
		 src/extensions/webember/Makefile
		 src/domain/Makefile
		 src/domain/bindings/Makefile
		 src/domain/bindings/lua/Makefile
		 data/Makefile
		 man/Makefile
		 man/man1/Makefile
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

