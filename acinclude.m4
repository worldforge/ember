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

# Configure paths for Libsigc++
# Based on Gtk-- script by Erik Andersen and Tero Pulkkinen 

dnl Test for Libsigc++, and define SIGC_CFLAGS and SIGC_LIBS
dnl   to be used as follows:
dnl AM_PATH_SIGC(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
AC_DEFUN(AM_PATH_SIGC,
[dnl 
dnl Get the cflags and libraries from the sigc-config script
dnl

dnl
dnl Prefix options
dnl
AC_ARG_WITH(sigc-prefix,
[  --with-sigc-prefix=PREFIX
                          Prefix where libsigc++ is installed (optional)]
, sigc_config_prefix="$withval", sigc_config_prefix="")

AC_ARG_WITH(sigc-exec-prefix,
[  --with-sigc-exec-prefix=PREFIX 
                          Exec prefix where  libsigc++ is installed (optional)]
, sigc_config_exec_prefix="$withval", sigc_config_exec_prefix="")

AC_ARG_ENABLE(sigctest, 
[  --disable-sigctest     Do not try to compile and run a test libsigc++ 
                          program],
, enable_sigctest=yes)

dnl
dnl Prefix handling
dnl
  if test x$sigc_config_exec_prefix != x ; then
     sigc_config_args="$sigc_config_args --exec-prefix=$sigc_config_exec_prefix"
     if test x${SIGC_CONFIG+set} != xset ; then
        SIGC_CONFIG=$sigc_config_exec_prefix/bin/sigc-config
     fi
  fi
  if test x$sigc_config_prefix != x ; then
     sigc_config_args="$sigc_config_args --prefix=$sigc_config_prefix"
     if test x${SIGC_CONFIG+set} != xset ; then
        SIGC_CONFIG=$sigc_config_prefix/bin/sigc-config
     fi
  fi

dnl
dnl See if sigc-config is alive
dnl
  AC_PATH_PROG(SIGC_CONFIG, sigc-config, no)
  sigc_version_min=$1

dnl
dnl  Version check
dnl
  AC_MSG_CHECKING(for libsigc++ - version >= $sigc_version_min)
  no_sigc=""
  if test "$SIGC_CONFIG" = "no" ; then
    no_sigc=yes
  else
    sigc_version=`$SIGC_CONFIG --version`

    SIGC_CFLAGS=`$SIGC_CONFIG $sigc_config_args --cflags`
    SIGC_LIBS=`$SIGC_CONFIG $sigc_config_args --libs`
    SIGC_MACROS=`$SIGC_CONFIG $sigc_config_args --macros`

    sigc_major_version=`echo $sigc_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sigc_minor_version=`echo $sigc_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sigc_micro_version=`echo $sigc_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    sigc_major_min=`echo $sigc_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sigc_minor_min=`echo $sigc_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sigc_micro_min=`echo $sigc_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    sigc_version_proper=`expr \
        $sigc_major_version \> $sigc_major_min \| \
        $sigc_major_version \= $sigc_major_min \& \
        $sigc_minor_version \> $sigc_minor_min \| \
        $sigc_major_version \= $sigc_major_min \& \
        $sigc_minor_version \= $sigc_minor_min \& \
        $sigc_micro_version \>= $sigc_micro_min `

    if test "$sigc_version_proper" = "1" ; then
      AC_MSG_RESULT([$sigc_major_version.$sigc_minor_version.$sigc_micro_version])
    else
      AC_MSG_RESULT(no)
      no_sigc=yes
    fi

    if test "X$no_sigc" = "Xyes" ; then
      enable_sigctest=no
    fi

    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

dnl
dnl
dnl
    if test "x$enable_sigctest" = "xyes" ; then
      AC_MSG_CHECKING(if libsigc++ sane)
      ac_save_CXXFLAGS="$CXXFLAGS"
      ac_save_LIBS="$LIBS"
      ac_exeext=""
      CXXFLAGS="$CXXFLAGS $SIGC_CFLAGS"
      LIBS="$LIBS $SIGC_LIBS"

      rm -f conf.sigctest
      AC_TRY_RUN([
#include <stdio.h>
#include <sigc++/signal_system.h>

#ifdef SIGC_CXX_NAMESPACES
using namespace SigC;
#endif

int foo1(int i) 
  {
   return 1;
  }

int main(int argc,char **argv)
  {
   if (sigc_major_version!=$sigc_major_version ||
       sigc_minor_version!=$sigc_minor_version ||
       sigc_micro_version!=$sigc_micro_version)
     { printf("(%d.%d.%d) ",
         sigc_major_version,sigc_minor_version,sigc_micro_version);
       return 1;
     }
   Signal1<int,int> sig1;
   sig1.connect(slot(foo1));
   sig1(1);
   return 0;
  }

],[
  AC_MSG_RESULT(yes)
],[
  AC_MSG_RESULT(no)
  no_sigc=yes
]
,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

       CXXFLAGS="$ac_save_CXXFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi

  dnl
  dnl
  if test "x$no_sigc" = x ; then
     ifelse([$2], , :, [$2])     
  else
     SIGC_CFLAGS=""
     SIGC_LIBS=""
     SIGC_MACROS=""
     ifelse([$3], , :, [$3])
  fi

  AC_LANG_RESTORE

  AC_SUBST(SIGC_CFLAGS)
  AC_SUBST(SIGC_LIBS)
  AC_SUBST(SIGC_MACROS)
])

# Configure paths for skstream
# Stolen by Michael Koch
# Adapted by Al Riddoch
# stolen from Sam Lantinga of SDL
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_SKSTREAM([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for skstream, and define SKSTREAM_CFLAGS and SKSTREAM_LIBS
dnl
AC_DEFUN(AM_PATH_SKSTREAM,
[dnl 
dnl Get the cflags and libraries from the skstream-config script
dnl
AC_LANG_CPLUSPLUS
AC_ARG_WITH(skstream-prefix,[  --with-skstream-prefix=PREFIX
                          Prefix where skstream is installed (optional)],
            skstream_prefix="$withval", skstream_prefix="")
AC_ARG_WITH(skstream-exec-prefix,[  --with-skstream-exec-prefix=PREFIX
                          Exec prefix where skstream is installed (optional)],
            skstream_exec_prefix="$withval", skstream_exec_prefix="")
AC_ARG_ENABLE(skstreamtest, [  --disable-skstreamtest     Do not try to compile and run a test skstream program],
		    , enable_skstreamtest=yes)

  if test x$skstream_exec_prefix != x ; then
     skstream_args="$skstream_args --exec-prefix=$skstream_exec_prefix"
     if test x${SKSTREAM_CONFIG+set} != xset ; then
        SKSTREAM_CONFIG=$skstream_exec_prefix/bin/skstream-config
     fi
  fi
  if test x$skstream_prefix != x ; then
     skstream_args="$skstream_args --prefix=$skstream_prefix"
     if test x${SKSTREAM_CONFIG+set} != xset ; then
        SKSTREAM_CONFIG=$skstream_prefix/bin/skstream-config
     fi
  fi

  AC_PATH_PROG(SKSTREAM_CONFIG, skstream-config, no)
  min_skstream_version=ifelse([$1], ,0.3.11,$1)
  AC_MSG_CHECKING(for skstream - version >= $min_skstream_version)
  no_skstream=""
  if test "$SKSTREAM_CONFIG" = "no" ; then
    no_skstream=yes
  else
    SKSTREAM_CFLAGS=`$SKSTREAM_CONFIG $skstreamconf_args --cflags`
    SKSTREAM_LIBS=`$SKSTREAM_CONFIG $skstreamconf_args --libs`

    skstream_major_version=`$SKSTREAM_CONFIG $skstream_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    skstream_minor_version=`$SKSTREAM_CONFIG $skstream_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    skstream_micro_version=`$SKSTREAM_CONFIG $skstream_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_skstreamtest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $SKSTREAM_CFLAGS"
      LIBS="$LIBS $SKSTREAM_LIBS"
dnl
dnl Now check if the installed skstream is sufficiently new. (Also sanity
dnl checks the results of skstream-config to some extent)
dnl
      rm -f conf.skstreamtest
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
  system ("touch conf.skstreamtest");
  */
  { FILE *fp = fopen("conf.skstreamtest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_skstream_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_skstream_version");
     exit(1);
   }

   if (($skstream_major_version > major) ||
      (($skstream_major_version == major) && ($skstream_minor_version > minor)) ||
      (($skstream_major_version == major) && ($skstream_minor_version == minor) && ($skstream_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'skstream-config --version' returned %d.%d.%d, but the minimum version\n", $skstream_major_version, $skstream_minor_version, $skstream_micro_version);
      printf("*** of skstream required is %d.%d.%d. If skstream-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If skstream-config was wrong, set the environment variable SKSTREAM_CONFIG\n");
      printf("*** to point to the correct copy of skstream-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_skstream=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_skstream" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$SKSTREAM_CONFIG" = "no" ; then
       echo "*** The skstream-config script installed by skstream could not be found"
       echo "*** If skstream was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the SKSTREAM_CONFIG environment variable to the"
       echo "*** full path to skstream-config."
     else
       if test -f conf.skstreamtest ; then
        :
       else
          echo "*** Could not run skstream test program, checking why..."
          CFLAGS="$CFLAGS $SKSTREAM_CFLAGS"
          LIBS="$LIBS $SKSTREAM_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding skstream or finding the wrong"
          echo "*** version of skstream. If it is not finding skstream, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means skstream was incorrectly installed"
          echo "*** or that you have moved skstream since it was installed. In the latter case, you"
          echo "*** may want to edit the skstream-config script: $SKSTREAM_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     SKSTREAM_CFLAGS=""
     SKSTREAM_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(SKSTREAM_CFLAGS)
  AC_SUBST(SKSTREAM_LIBS)
  rm -f conf.skstreamtest
])

# Configure paths for Atlas
# Adapted by Al Riddoch
# stolen from Sam Lantinga of SDL
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_ATLAS([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for Atlas, and define ATLAS_CFLAGS and ATLAS_LIBS
dnl
AC_DEFUN(AM_PATH_ATLAS,
[dnl 
dnl Get the cflags and libraries from the atlas-config script
dnl
AC_LANG_CPLUSPLUS
AC_ARG_WITH(atlas-prefix,[  --with-atlas-prefix=PFX   Prefix where Atlas is installed (optional)],
            atlas_prefix="$withval", atlas_prefix="")
AC_ARG_WITH(atlas-exec-prefix,[  --with-atlas-exec-prefix=PFX Exec prefix where Atlas is installed (optional)],
            atlas_exec_prefix="$withval", atlas_exec_prefix="")
AC_ARG_ENABLE(atlastest, [  --disable-atlastest       Do not try to compile and run a test Atlas program],
		    , enable_atlastest=yes)

  if test x$atlas_exec_prefix != x ; then
     atlas_args="$atlas_args --exec-prefix=$atlas_exec_prefix"
     if test x${ATLAS_CONFIG+set} != xset ; then
        ATLAS_CONFIG=$atlas_exec_prefix/bin/atlas-config
     fi
  fi
  if test x$atlas_prefix != x ; then
     atlas_args="$atlas_args --prefix=$atlas_prefix"
     if test x${ATLAS_CONFIG+set} != xset ; then
        ATLAS_CONFIG=$atlas_prefix/bin/atlas-config
     fi
  fi

  AC_PATH_PROG(ATLAS_CONFIG, atlas-config, no)
  min_atlas_version=ifelse([$1], ,0.3.11,$1)
  AC_MSG_CHECKING(for Atlas - version >= $min_atlas_version)
  no_atlas=""
  if test "$ATLAS_CONFIG" = "no" ; then
    no_atlas=yes
  else
    ATLAS_CFLAGS=`$ATLAS_CONFIG $atlasconf_args --cflags`
    ATLAS_LIBS=`$ATLAS_CONFIG $atlasconf_args --libs`

    atlas_major_version=`$ATLAS_CONFIG $atlas_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    atlas_minor_version=`$ATLAS_CONFIG $atlas_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    atlas_micro_version=`$ATLAS_CONFIG $atlas_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_atlastest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $ATLAS_CFLAGS"
      LIBS="$LIBS $ATLAS_LIBS"
dnl
dnl Now check if the installed Atlas is sufficiently new. (Also sanity
dnl checks the results of atlas-config to some extent)
dnl
      rm -f conf.atlastest
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
  system ("touch conf.atlastest");
  */
  { FILE *fp = fopen("conf.atlastest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_atlas_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_atlas_version");
     exit(1);
   }

   if (($atlas_major_version > major) ||
      (($atlas_major_version == major) && ($atlas_minor_version > minor)) ||
      (($atlas_major_version == major) && ($atlas_minor_version == minor) && ($atlas_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'atlas-config --version' returned %d.%d.%d, but the minimum version\n", $atlas_major_version, $atlas_minor_version, $atlas_micro_version);
      printf("*** of Atlas required is %d.%d.%d. If atlas-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If atlas-config was wrong, set the environment variable ATLAS_CONFIG\n");
      printf("*** to point to the correct copy of atlas-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_atlas=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_atlas" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$ATLAS_CONFIG" = "no" ; then
       echo "*** The atlas-config script installed by Atlas could not be found"
       echo "*** If Atlas was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the ATLAS_CONFIG environment variable to the"
       echo "*** full path to atlas-config."
     else
       if test -f conf.atlastest ; then
        :
       else
          echo "*** Could not run Atlas test program, checking why..."
          CFLAGS="$CFLAGS $ATLAS_CFLAGS"
          LIBS="$LIBS $ATLAS_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding Atlas or finding the wrong"
          echo "*** version of Atlas. If it is not finding Atlas, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means Atlas was incorrectly installed"
          echo "*** or that you have moved Atlas since it was installed. In the latter case, you"
          echo "*** may want to edit the atlas-config script: $ATLAS_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     ATLAS_CFLAGS=""
     ATLAS_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(ATLAS_CFLAGS)
  AC_SUBST(ATLAS_LIBS)
  rm -f conf.atlastest
])
