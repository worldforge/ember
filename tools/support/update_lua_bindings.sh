#!/bin/sh
#tolua++ will for some reason translate "const std::string" into "const,std::string", so we need to remove these incorrect commas from the final code
#some versions will also for some unexplainable reason not correctly remove the tolua specific directive tolua_outside, so we need to clean that out also
#We'll also replace the inclusion of "tolua++.h" with our own version which has better support for building on win32.
echo "Updating lua bindings."
set -e

#If the TOLUAXX environment variable isn't set default to using the command "tolua++".
if [ x${TOLUAXX} = x ]; then
	TOLUAXX=tolua++
fi


if command -v ${TOLUAXX} &> /dev/null; then
    ${TOLUAXX} $2 -n $1 > $3
    grep -q '** tolua internal error' $3 && cat $3 && exit 1
    if [ "`echo $OSTYPE | grep darwin`" != "" ] ; then
        sed -i "" -e 's/const,/const /g' -e 's/tolua_outside//g' -e 's/tolua++\.h/components\/lua\/embertolua++\.h/' -e 's/self->operator==(\*value)/(\*self == \*value)/g' $3
    else
        sed -i -e 's/const,/const /g' -e 's/tolua_outside//g' -e 's/tolua++\.h/components\/lua\/embertolua++\.h/' -e 's/self->operator==(\*value)/(\*self == \*value)/g' $3
    fi
else
    echo "Could not find tolua++ command."
    exit -1
fi