dnl
dnl AM_PATH_CPPUNIT([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
AC_DEFUN(AM_PATH_CPPUNIT,
[

AC_ARG_WITH(cppunit-prefix,[  --with-cppunit-prefix=PFX   Prefix where CppUnit is installed (optional)],
            cppunit_config_prefix="$withval", cppunit_config_prefix="")
AC_ARG_WITH(cppunit-exec-prefix,[  --with-cppunit-exec-prefix=PFX  Exec prefix where CppUnit is installed (optional)],
            cppunit_config_exec_prefix="$withval", cppunit_config_exec_prefix="")

  if test x$cppunit_config_exec_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --exec-prefix=$cppunit_config_exec_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_exec_prefix/bin/cppunit-config
     fi
  fi
  if test x$cppunit_config_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --prefix=$cppunit_config_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_prefix/bin/cppunit-config
     fi
  fi

  AC_PATH_PROG(CPPUNIT_CONFIG, cppunit-config, no)
  cppunit_version_min=$1

  AC_MSG_CHECKING(for Cppunit - version >= $cppunit_version_min)
  no_cppunit=""
  if test "$CPPUNIT_CONFIG" = "no" ; then
    no_cppunit=yes
  else
    CPPUNIT_CFLAGS=`$CPPUNIT_CONFIG --cflags`
    CPPUNIT_LIBS=`$CPPUNIT_CONFIG --libs`
    cppunit_version=`$CPPUNIT_CONFIG --version`

    cppunit_major_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_major_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_version_proper=`expr \
        $cppunit_major_version \> $cppunit_major_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \> $cppunit_minor_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \= $cppunit_minor_min \& \
        $cppunit_micro_version \>= $cppunit_micro_min `

    if test "$cppunit_version_proper" = "1" ; then
      AC_MSG_RESULT([$cppunit_major_version.$cppunit_minor_version.$cppunit_micro_version])
    else
      AC_MSG_RESULT(no)
      no_cppunit=yes
    fi
  fi

  if test "x$no_cppunit" = x ; then
     ifelse([$2], , :, [$2])     
  else
     CPPUNIT_CFLAGS=""
     CPPUNIT_LIBS=""
     ifelse([$3], , :, [$3])
  fi

  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
])

dnl worldforge check for libz, libpng, SDL and SDL_image
dnl the flags are returned in WF_SDL_IMAGE_CFLAGS and WF_SDL_IMAGE_LIBS
AC_DEFUN(WF_CHECK_SDL_IMAGE,[dnl

AC_LANG_PUSH([C])

dnl save CFLAGS and LIBS, so we can munge them when calling tests

WF_SDL_IMAGE_TMP_CFLAGS="$CFLAGS"
WF_SDL_IMAGE_TMP_LIBS="$LIBS"
WF_SDL_IMAGE_TMP_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $LIBS"
LIBS=""

dnl check for libpng prefix option
AC_ARG_WITH(png-prefix,[  --with-png-prefix=PFX   Prefix where libpng is installed (option
al)],[dnl
	WF_SDL_IMAGE_CFLAGS="$WF_SDL_IMAGE_CFLAGS -I$withval/include"
	WF_SDL_IMAGE_LIBS="$WF_SDL_IMAGE_LIBS -L$withval/lib"
],[])

dnl check for libz prefix configure option
AC_ARG_WITH(z-prefix,[  --with-z-prefix=PFX     Prefix where libz is installed (optional)]
,[dnl
	WF_SDL_IMAGE_CFLAGS="$WF_SDL_IMAGE_CFLAGS -I$withval/include"
	WF_SDL_IMAGE_LIBS="$WF_SDL_IMAGE_LIBS -L$withval/lib"
],[])

CFLAGS="$CFLAGS $WF_SDL_IMAGE_CFLAGS"
LDFLAGS="$LDFLAGS $WF_SDL_IMAGE_LIBS"

dnl FIXME AC_CHECK_LIB() doesn't put -lfoo after the -L args
dnl we've inserted in LIBS, this may break on some platforms.

dnl check for libpng and libz

AC_CHECK_HEADER(zlib.h,[],AC_MSG_ERROR( You need libz headers for SDL_image!))
LIBS="-lz $LIBS"
AC_MSG_CHECKING(for libz)
AC_TRY_LINK([
		#include <zlib.h>
	],[
		int main()
		{
			zlibVersion();
			return 0;
		}
	],[
		AC_MSG_RESULT(yes)
	],[
		AC_MSG_RESULT(no)
		AC_MSG_ERROR( You need libz for SDL_image!)
	])


AC_CHECK_HEADER(png.h,[],[AC_MSG_ERROR( You need libpng headers for SDL_image!)])
LIBS="-lpng $LIBS"
AC_MSG_CHECKING(for libpng)
AC_TRY_LINK([
		#include <png.h>
	],[
		int main()
		{
			png_access_version_number();
			return 0;
		}
	],[
		AC_MSG_RESULT(yes)
	],[
		AC_MSG_RESULT(no)
		AC_MSG_ERROR( You need libpng for SDL_image!)
	])

WF_SDL_IMAGE_LIBS="$WF_SDL_IMAGE_LIBS -lpng -lz"

dnl Check for SDL and SDL_image

AM_PATH_SDL($1,,AC_MSG_ERROR([Couldn't find libSDL $1 or greater]))

dnl We pull in SDL_LIBS twice, to get the -L args before
dnl SDL_LIBS, but the -l args after as well. We pull in
dnl -lSDL_image here as well, so we can test for it
dnl in the next check.
WF_SDL_IMAGE_CFLAGS="$SDL_CFLAGS $WF_SDL_IMAGE_CFLAGS"
WF_SDL_IMAGE_LIBS="$SDL_LIBS -lSDL_image $SDL_LIBS $WF_SDL_IMAGE_LIBS"

CFLAGS="$WF_SDL_IMAGE_TMP_CFLAGS $WF_SDL_IMAGE_CFLAGS"
LIBS="$WF_SDL_IMAGE_TMP_LIBS $WF_SDL_IMAGE_LIBS"
LDFLAGS="$WF_SDL_IMAGE_TMP_LDFLAGS"

AC_CHECK_HEADER(SDL/SDL_image.h,[],[AC_MSG_ERROR(Missing SDL_image.h)])

dnl Use AC_TRY_LINK for a custom SDL_image check
AC_MSG_CHECKING(for SDL_image)
AC_TRY_LINK([
		#include <SDL/SDL_image.h>
		// use our own main(), so SDL_main links properly
		#if 0
	],[
		#endif
		int main(int argc, char** argv) {
		IMG_Load(0);
	],[
		AC_MSG_RESULT(yes)
	],[
		AC_MSG_RESULT(no)
		AC_MSG_ERROR(Couldn't link libSDL_image)
	])

dnl Got it, now clean up

CFLAGS="$WF_SDL_IMAGE_TMP_CFLAGS"
LIBS="$WF_SDL_IMAGE_TMP_LIBS"
LDFLAGS="$WF_SDL_IMAGE_TMP_LDFLAGS"

AC_LANG_POP([C])

])dnl end of WF_CHECK_SDL_IMAGE

dnl worldforge check for the GL library,
dnl the -l flag is added directly to LIBS

AC_DEFUN(WF_CHECK_GL_SINGLE_LIB,[dnl

AC_LANG_PUSH(C)

AC_CHECK_LIB(GL,glViewport, ,
    AC_MSG_CHECKING(for glViewport in opengl32)
    LIBS="$LIBS -lopengl32"
    AC_TRY_LINK([
	#include <GL/gl.h>
    ],[
	glViewport(0, 0, 0, 0);
    ],[
	AC_MSG_RESULT(yes)
    ],[
	AC_MSG_RESULT(no)
	AC_MSG_ERROR(Could not find OpenGL library)
    ])
)

AC_LANG_POP(C)

])dnl end of WF_CHECK_GL_LIBS
