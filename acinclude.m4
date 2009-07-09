dnl
dnl AM_PATH_CPPUNIT([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
AC_DEFUN([AM_PATH_CPPUNIT],
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

# Check for binary relocation support.
# Written by Hongli Lai
# http://autopackage.org/

AC_DEFUN([AM_BINRELOC],
[
	AC_ARG_ENABLE(binreloc,
		[  --enable-binreloc       compile with binary relocation support
                          (default=enable when available)],
		enable_binreloc=$enableval,enable_binreloc=auto)

	AC_ARG_ENABLE(binreloc-threads,
		[  --enable-binreloc-threads      compile binary relocation with threads support
	                         (default=yes)],
		enable_binreloc_threads=$enableval,enable_binreloc_threads=yes)

	BINRELOC_CFLAGS=
	BINRELOC_LIBS=
	if test "x$enable_binreloc" = "xauto"; then
		AC_CHECK_FILE([/proc/self/maps])
		AC_CACHE_CHECK([whether everything is installed to the same prefix],
			       [br_cv_valid_prefixes], [
				if test "$bindir" = '${exec_prefix}/bin' -a "$sbindir" = '${exec_prefix}/sbin' -a \
					"$datadir" = '${prefix}/share' -a "$libdir" = '${exec_prefix}/lib' -a \
					"$libexecdir" = '${exec_prefix}/libexec' -a "$sysconfdir" = '${prefix}/etc'
				then
					br_cv_valid_prefixes=yes
				else
					br_cv_valid_prefixes=no
				fi
				])
	fi
	AC_CACHE_CHECK([whether binary relocation support should be enabled],
		       [br_cv_binreloc],
		       [if test "x$enable_binreloc" = "xyes"; then
		       	       br_cv_binreloc=yes
		       elif test "x$enable_binreloc" = "xauto"; then
			       if test "x$br_cv_valid_prefixes" = "xyes" -a \
			       	       "x$ac_cv_file__proc_self_maps" = "xyes"; then
				       br_cv_binreloc=yes
			       else
				       br_cv_binreloc=no
			       fi
		       else
			       br_cv_binreloc=no
		       fi])

	if test "x$br_cv_binreloc" = "xyes"; then
		BINRELOC_CFLAGS="-DENABLE_BINRELOC"
		AC_DEFINE(ENABLE_BINRELOC,,[Use binary relocation?])
		if test "x$enable_binreloc_threads" = "xyes"; then
			AC_CHECK_LIB([pthread], [pthread_getspecific])
		fi

		AC_CACHE_CHECK([whether binary relocation should use threads],
			       [br_cv_binreloc_threads],
			       [if test "x$enable_binreloc_threads" = "xyes"; then
					if test "x$ac_cv_lib_pthread_pthread_getspecific" = "xyes"; then
						br_cv_binreloc_threads=yes
					else
						br_cv_binreloc_threads=no
					fi
			        else
					br_cv_binreloc_threads=no
				fi])

		if test "x$br_cv_binreloc_threads" = "xyes"; then
			BINRELOC_LIBS="-lpthread"
			AC_DEFINE(BR_PTHREAD,1,[Include pthread support for binary relocation?])
		else
			BINRELOC_CFLAGS="$BINRELOC_CFLAGS -DBR_PTHREADS=0"
			AC_DEFINE(BR_PTHREAD,0,[Include pthread support for binary relocation?])
		fi
	fi
	AC_SUBST(BINRELOC_CFLAGS)
	AC_SUBST(BINRELOC_LIBS)
])
