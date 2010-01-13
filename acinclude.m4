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

dnl worldforge check for the GL library,
dnl the -l flag is added directly to LIBS

AC_DEFUN([WF_CHECK_GL_SINGLE_LIB],[dnl

AC_LANG_PUSH(C)

dnl AC_MSG_CHECKING(for glViewport in libGL)

AC_CHECK_LIB([GL], [glViewport],[
    LIBS="$LIBS -lGL"
	CXXFLAGS="$CXXFLAGS -I/usr/X11R6/include -I/usr/include -DHAVE_OPENGL"
    ],[
	AC_MSG_NOTICE([Could not find OpenGL library. This only means that we can't check for indirect rendering.])
    ]
)

AC_LANG_POP(C)

])dnl end of WF_CHECK_GL_LIBS


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



#   Copyright (c) 2007 Thomas Porschberg <thomas@randspringer.de>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.
AC_DEFUN([AX_BOOST],
[
    AC_ARG_WITH([boost],
                AS_HELP_STRING([--with-boost=DIR],
                [use boost (default is yes) specify the root directory for boost library (optional)]),
                [
                if test "$withval" = "no"; then
		            want_boost="no"
                elif test "$withval" = "yes"; then
                    want_boost="yes"
                    ac_boost_path=""
                else
			        want_boost="yes"
            		ac_boost_path="$withval"
		        fi
            	],
                [want_boost="yes"])

    AC_CANONICAL_BUILD
	if test "x$want_boost" = "xyes"; then
        AC_REQUIRE([AC_PROG_CC])
		boost_lib_version_req=ifelse([$1], ,1.31.0,$1)
		boost_lib_version_req_shorten=`expr $boost_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
		boost_lib_version_req_major=`expr $boost_lib_version_req : '\([[0-9]]*\)'`
		boost_lib_version_req_minor=`expr $boost_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
		boost_lib_version_req_sub_minor=`expr $boost_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
		if test "x$boost_lib_version_req_sub_minor" = "x" ; then
			boost_lib_version_req_sub_minor="0"
    	fi
		WANT_BOOST_VERSION=`expr $boost_lib_version_req_major \* 100000 \+  $boost_lib_version_req_minor \* 100 \+ $boost_lib_version_req_sub_minor`
		AC_MSG_CHECKING(for boostlib >= $boost_lib_version_req)
		succeeded=no

		dnl first we check the system location for boost libraries
		dnl this location ist chosen if boost libraries are installed with the --layout=system option
		dnl or if you install boost with RPM
		if test "$ac_boost_path" != ""; then
			BOOST_LDFLAGS="-L$ac_boost_path/lib"
			BOOST_CPPFLAGS="-I$ac_boost_path/include"
		else
			for ac_boost_path_tmp in /usr /usr/local /opt ; do
				if test -d "$ac_boost_path_tmp/include/boost" && test -r "$ac_boost_path_tmp/include/boost"; then
					BOOST_LDFLAGS="-L$ac_boost_path_tmp/lib"
					BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
					break;
				fi
			done
		fi

		CPPFLAGS_SAVED="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
		export CPPFLAGS

		LDFLAGS_SAVED="$LDFLAGS"
		LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
		export LDFLAGS

	AC_LANG_PUSH(C++)
     	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]],
       [[
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
#  error Boost version is too old
#endif

		]])],
    	[
         AC_MSG_RESULT(yes)
		 succeeded=yes
		 found_system=yes
         ifelse([$2], , :, [$2])
       ],
       [
       ])
       AC_LANG_POP([C++])
		dnl if we found no boost with system layout we search for boost libraries
		dnl built and installed without the --layout=system option or for a staged(not installed) version
		if test "x$succeeded" != "xyes"; then
			_version=0
			if test "$ac_boost_path" != ""; then
                BOOST_LDFLAGS="-L$ac_boost_path/lib"
				if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
					for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
						_version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
						V_CHECK=`expr $_version_tmp \> $_version`
						if test "$V_CHECK" = "1" ; then
							_version=$_version_tmp
						fi
						VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
						BOOST_CPPFLAGS="-I$ac_boost_path/include/boost-$VERSION_UNDERSCORE"
					done
				fi
			else
				for ac_boost_path in /usr /usr/local /opt ; do
					if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
						for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
							_version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
							V_CHECK=`expr $_version_tmp \> $_version`
							if test "$V_CHECK" = "1" ; then
								_version=$_version_tmp
								best_path=$ac_boost_path
							fi
						done
					fi
				done

				VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
				BOOST_CPPFLAGS="-I$best_path/include/boost-$VERSION_UNDERSCORE"
				BOOST_LDFLAGS="-L$best_path/lib"

	    		if test "x$BOOST_ROOT" != "x"; then
                    if test -d "$BOOST_ROOT" && test -r "$BOOST_ROOT" && test -d "$BOOST_ROOT/stage/lib" && test -r "$BOOST_ROOT/stage/lib"; then
						version_dir=`expr //$BOOST_ROOT : '.*/\(.*\)'`
						stage_version=`echo $version_dir | sed 's/boost_//' | sed 's/_/./g'`
						stage_version_shorten=`expr $stage_version : '\([[0-9]]*\.[[0-9]]*\)'`
						V_CHECK=`expr $stage_version_shorten \>\= $_version`
						if test "$V_CHECK" = "1" ; then
							AC_MSG_NOTICE(We will use a staged boost library from $BOOST_ROOT)
							BOOST_CPPFLAGS="-I$BOOST_ROOT"
							BOOST_LDFLAGS="-L$BOOST_ROOT/stage/lib"
						fi
					fi
	    		fi
			fi

			CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
			export CPPFLAGS
			LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
			export LDFLAGS

            AC_LANG_PUSH(C++)
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]],
       [[
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
#  error Boost version is too old
#endif

		]])],
    	[
         AC_MSG_RESULT(yes ($_version))
		 succeeded=yes
         ifelse([$2], , :, [$2])
       ],
       [
         AC_MSG_RESULT(no ($_version))
         ifelse([$3], , :, [$3])
       ])
    	AC_LANG_POP([C++])
		fi

		if test "$succeeded" != "yes" ; then
			if test "$_version" = "0" ; then
				AC_MSG_ERROR([[We could not detect the boost libraries (version $boost_lib_version_req_shorten or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
			else
				AC_MSG_ERROR('Your boost libraries seems to old (version $_version).  We need at least $boost_lib_version_shorten')
			fi
		else
			AC_SUBST(BOOST_CPPFLAGS)
			AC_SUBST(BOOST_LDFLAGS)
			AC_DEFINE(HAVE_BOOST,,[define if the Boost library is available])


			AC_CACHE_CHECK(whether the Boost::Thread library is available,
						   ax_cv_boost_thread,
						[AC_LANG_PUSH([C++])
			 CXXFLAGS_SAVE=$CXXFLAGS

			 if test "x$build_os" = "xsolaris" ; then
  				 CXXFLAGS="-pthreads $CXXFLAGS"
			 elif test "x$build_os" = "xming32" ; then
				 CXXFLAGS="-mthreads $CXXFLAGS"
			 else
				CXXFLAGS="-pthread $CXXFLAGS"
			 fi
			 AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/thread/thread.hpp>]],
                                   [[boost::thread_group thrds;
                                   return 0;]]),
                   ax_cv_boost_thread=yes, ax_cv_boost_thread=no)
			 CXXFLAGS=$CXXFLAGS_SAVE
             AC_LANG_POP([C++])
			])
			if test "x$ax_cv_boost_thread" = "xyes"; then
               if test "x$build_os" = "xsolaris" ; then
 				  BOOST_CPPFLAGS="-pthreads $BOOST_CPPFLAGS"
			   elif test "x$build_os" = "xming32" ; then
 				  BOOST_CPPFLAGS="-mthreads $BOOST_CPPFLAGS"
			   else
				  BOOST_CPPFLAGS="-pthread $BOOST_CPPFLAGS"
			   fi

				AC_SUBST(BOOST_CPPFLAGS)
				AC_DEFINE(HAVE_BOOST_THREAD,,[define if the Boost::THREAD library is available])
				BN=boost_thread
     			LDFLAGS_SAVE=$LDFLAGS
                        case "x$build_os" in
                          *bsd* )
                               LDFLAGS="-pthread $LDFLAGS"
                          break;
                          ;;
                        esac
                echo $CC
                TMPCC=$CC`$CC -dumpversion |sed  -e s/@<:@^a-zA-Z0-9@:>@//g|sed -e s/.$//g`
                echo $TMPCC
				for ax_lib in $BN-$TMPCC $BN-$TMPCC-mt $BN-$TMPCC-mt-s $BN-$TMPCC-s $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s $BN $BN-mt \
                              lib$BN-$TMPCC lib$BN-$TMPCC-mt lib$BN-$TMPCC-mt-s lib$BN-$TMPCC-s lib$BN-$CC lib$BN-$CC-mt lib$BN-$CC-mt-s lib$BN-$CC-s lib$BN lib$BN-mt \
                              $BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s ; do
				    AC_CHECK_LIB($ax_lib, main, [BOOST_THREAD_LIB="-l$ax_lib"; AC_SUBST(BOOST_THREAD_LIB) link_thread="yes"; break],
                                 [link_thread="no"])
  				done
				if test "x$link_thread" = "xno"; then
					AC_MSG_NOTICE(Could not link against $ax_lib !)
                else
                    case "x$build_os" in
                       *bsd* )
                       BOOST_LDFLAGS="-pthread $BOOST_LDFLAGS"
                       break;
                       ;;
                    esac
				fi
			fi
		fi
        CPPFLAGS="$CPPFLAGS_SAVED"
        LDFLAGS="$LDFLAGS_SAVED"
	fi
])


AC_DEFUN([WF_CHECK_THREADING],
[

	AC_REQUIRE([AC_PROG_CC])
	AC_REQUIRE([AC_CANONICAL_BUILD])
	AX_BOOST
	CPPFLAGS_SAVED="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	export CPPFLAGS
	
	LDFLAGS_SAVED="$LDFLAGS"
	LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
	export LDFLAGS
	
	AC_CACHE_CHECK(whether the Boost::Thread library is available,
				   ax_cv_boost_thread,
	[AC_LANG_PUSH([C++])
		 CXXFLAGS_SAVE=$CXXFLAGS
	
		 if test "x$build_os" = "xsolaris" ; then
			 CXXFLAGS="-pthreads $CXXFLAGS"
		 elif test "x$build_os" = "xming32" ; then
			 CXXFLAGS="-mthreads $CXXFLAGS"
		 else
			CXXFLAGS="-pthread $CXXFLAGS"
		 fi
		 AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/thread/thread.hpp>]],
	                           [[boost::thread_group thrds;
	                           return 0;]]),
	           ax_cv_boost_thread=yes, ax_cv_boost_thread=no)
		 CXXFLAGS=$CXXFLAGS_SAVE
	     AC_LANG_POP([C++])
	])
	if test "x$ax_cv_boost_thread" = "xyes"; then
	   if test "x$build_os" = "xsolaris" ; then
		  BOOST_CPPFLAGS="-pthreads $BOOST_CPPFLAGS"
	   elif test "x$build_os" = "xming32" ; then
		  BOOST_CPPFLAGS="-mthreads $BOOST_CPPFLAGS"
	   else
		  BOOST_CPPFLAGS="-pthread $BOOST_CPPFLAGS"
	   fi
	
		AC_SUBST(BOOST_CPPFLAGS)
	
		AC_DEFINE(HAVE_BOOST_THREAD,,[define if the Boost::Thread library is available])
		BN=boost_thread
	
		LDFLAGS_SAVE=$LDFLAGS
	                case "x$build_os" in
	                  *bsd* )
	                       LDFLAGS="-pthread $LDFLAGS"
	                  break;
	                  ;;
	                esac
	    TMPCC=$CC`$CC -dumpversion |sed  -e s/@<:@^a-zA-Z0-9@:>@//g|sed -e s/.$//g`
	    if test "x$ax_boost_user_thread_lib" = "x"; then
			for ax_lib in $BN-$TMPCC $BN-$TMPCC-mt $BN-$TMPCC-mt-s $BN-$TMPCC-s $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s $BN $BN-mt \
	                      lib$BN-$TMPCC lib$BN-$TMPCC-mt lib$BN-$TMPCC-mt-s lib$BN-$TMPCC-s lib$BN-$CC lib$BN-$CC-mt lib$BN-$CC-mt-s lib$BN-$CC-s lib$BN lib$BN-mt \
	                      $BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s ; do
			    AC_CHECK_LIB($ax_lib, main, [BOOST_THREAD_LIB="-l$ax_lib"; AC_SUBST(BOOST_THREAD_LIB) link_thread="yes"; break],
	                         [link_thread="no"])
			done
	    else
	       for ax_lib in $ax_boost_user_thread_lib $BN-$ax_boost_user_thread_lib; do
			      AC_CHECK_LIB($ax_lib, main,
	                           [BOOST_THREAD_LIB="-l$ax_lib"; AC_SUBST(BOOST_THREAD_LIB) link_thread="yes"; break],
	                           [link_thread="no"])
	          done
	
	    fi
		if test "x$link_thread" = "xno"; then
			AC_MSG_ERROR(Could not link against $ax_lib !)
	                else
	                   case "x$build_os" in
	                      *bsd* )
		        LDFLAGS="-pthread $LDFLAGS"
	                      break;
	                      ;;
	                   esac
	
		fi
	fi

	#STLPORT BECAUSE THE LIBRARIES DONT HAVE ANY WAY TO GET C++ FLAGS PASSED INTO THEM SO WE CAN TELL THEM WHERE BOOST LIVES
	AC_SUBST(STLPORT_CFLAGS)
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	LDFLAGS="$LDFLAGS $BOOST_THREAD_LIB"
])
