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

# Configure paths for Eris
# Adapted by Michael Kocg from Eris-C++
# Adapted by Al Riddoch
# stolen from Sam Lantinga of SDL
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_ERIS([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for Eris, and define ERIS_CFLAGS and ERIS_LIBS
dnl
AC_DEFUN(AM_PATH_ERIS,
[dnl 
dnl Get the cflags and libraries from the eris-config script
dnl
AC_LANG_CPLUSPLUS
AC_ARG_WITH(eris-prefix,[  --with-eris-prefix=PREFIX
                          Prefix where Eris is installed (optional)],
            eris_prefix="$withval", eris_prefix="")
AC_ARG_WITH(eris-exec-prefix,[  --with-eris-exec-prefix=PREFIX
                          Exec prefix where Eris is installed (optional)],
            eris_exec_prefix="$withval", eris_exec_prefix="")
AC_ARG_ENABLE(eristest, [  --disable-eristest     Do not try to compile and run a test Eris program],
		    , enable_eristest=yes)

  if test x$eris_exec_prefix != x ; then
     eris_args="$eris_args --exec-prefix=$eris_exec_prefix"
     if test x${ERIS_CONFIG+set} != xset ; then
        ERIS_CONFIG=$eris_exec_prefix/bin/eris-config
     fi
  fi
  if test x$eris_prefix != x ; then
     eris_args="$eris_args --prefix=$eris_prefix"
     if test x${ERIS_CONFIG+set} != xset ; then
        ERIS_CONFIG=$eris_prefix/bin/eris-config
     fi
  fi

  AC_PATH_PROG(ERIS_CONFIG, eris-config, no)
  min_eris_version=ifelse([$1], ,0.3.11,$1)
  AC_MSG_CHECKING(for Eris - version >= $min_eris_version)
  no_eris=""
  if test "$ERIS_CONFIG" = "no" ; then
    no_eris=yes
  else
    ERIS_CFLAGS=`$ERIS_CONFIG $erisconf_args --cflags`
    ERIS_LIBS=`$ERIS_CONFIG $erisconf_args --libs`

    eris_major_version=`$ERIS_CONFIG $eris_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    eris_minor_version=`$ERIS_CONFIG $eris_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    eris_micro_version=`$ERIS_CONFIG $eris_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_eristest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $ERIS_CFLAGS"
      LIBS="$LIBS $ERIS_LIBS"
dnl
dnl Now check if the installed Eris is sufficiently new. (Also sanity
dnl checks the results of eris-config to some extent)
dnl
      rm -f conf.eristest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.eristest");
  */
  { FILE *fp = fopen("conf.eristest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_eris_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_eris_version");
     exit(1);
   }

   if (($eris_major_version > major) ||
      (($eris_major_version == major) && ($eris_minor_version > minor)) ||
      (($eris_major_version == major) && ($eris_minor_version == minor) && ($eris_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'eris-config --version' returned %d.%d.%d, but the minimum version\n", $eris_major_version, $eris_minor_version, $eris_micro_version);
      printf("*** of Eris required is %d.%d.%d. If eris-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If eris-config was wrong, set the environment variable ERIS_CONFIG\n");
      printf("*** to point to the correct copy of eris-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_eris=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_eris" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$ERIS_CONFIG" = "no" ; then
       echo "*** The eris-config script installed by Eris could not be found"
       echo "*** If Eris was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the ERIS_CONFIG environment variable to the"
       echo "*** full path to eris-config."
     else
       if test -f conf.eristest ; then
        :
       else
          echo "*** Could not run Eris test program, checking why..."
          CFLAGS="$CFLAGS $ERIS_CFLAGS"
          LIBS="$LIBS $ERIS_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding Eris or finding the wrong"
          echo "*** version of Eris. If it is not finding Eris, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means Eris was incorrectly installed"
          echo "*** or that you have moved Eris since it was installed. In the latter case, you"
          echo "*** may want to edit the eris-config script: $ERIS_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     ERIS_CFLAGS=""
     ERIS_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(ERIS_CFLAGS)
  AC_SUBST(ERIS_LIBS)
  rm -f conf.eristest
])
