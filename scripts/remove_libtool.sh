#!/bin/bash
for mfile in $(find . -name Makefile.am)
do
    echo ${mfile}
    sed -i "s/^noinst_LTLIBRARIES \(.*\)\.la/noinst_LIBRARIES \1.a/g" ${mfile}
    sed -i "s/\(lib[A-Za-z_]\+\)\.la/\1.a/g" ${mfile}
    sed -i "s/^\(lib[A-Za-z_]\+\)_la_/\1_a_/" ${mfile}
done
