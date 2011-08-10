#! /bin/sh
#A script for combining the legacy ChangeLog found in ChangeLog-CVS with one output from git. The script will only run if the current ChangeLog contains exactly one line.
#This makes it possible to do a new "make dist" from an existing source distribution (as the ChangeLog would then be complete.).
top_srcdir=$1
distdir=$2
commitid=$3
if [ x${distdir} = "x" ] || [ x${top_srcdir} = "x" ] || [ x${commitid} = "x" ]; then
	echo "This script will generate an aggregated ChangeLog by combining the legacy ChangeLog-CVS file with the output from git log. It therefore needs to be run in a git source directory."
	echo "Params: <source directory path> <distribution directory path> <SHA1 of first git commit>"	
	exit 1
fi
#Only do the aggregation if the ChangeLog file is exactly one line. If not the aggregation has already been done.
if [ `cat ${distdir}/ChangeLog | wc -l` = "0" ]; then
	echo "Generating ChangeLog by appending the old CVS ChangeLog to the one generated from git. This requires that you create the dist in the git repository."

	chmod u+w ${distdir}/ChangeLog && git log ${commitid}..HEAD --stat --name-only --date=short --abbrev-commit > ${distdir}/ChangeLog && echo "" >> ${distdir}/ChangeLog && cat ${top_srcdir}/ChangeLog-CVS >> ${distdir}/ChangeLog
	#Put a notice in the legacy ChangeLog-CVS file, thus bringing the size of the dist down a bit.
	chmod u+w ${distdir}/ChangeLog-CVS && echo "This file was needed for generating the proper ChangeLog as an aggregate of the code held in git and older code in CVS. It's now empty, but needs to be included in the source distribution to not upset automake." > ${distdir}/ChangeLog-CVS
fi
