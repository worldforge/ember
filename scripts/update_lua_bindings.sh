#! /bin/sh
#tolua++ will for some reason translate "const std::string" into "const,std::string", so we need to remove these incorrect commas from the final code
#some versions will also for some unexplainable reason not correctly remove the tolua specific directive tolua_outside, so we need to clean that out also
#stripping date to not having conflicts in version control system on regeneration
#We'll also replace the inclusion of "tolua++.h" with our own version which has better support for building on win32.
echo "Updating lua bindings."

#If the TOLUAXX environment variable isn't set default to using the command "tolua++".
if [ x${TOLUAXX} = x ]; then
	TOLUAXX=tolua++
fi
${TOLUAXX} -n $1 $2 > $3
grep -q '** tolua internal error' $3 && cat $3 && exit 1
sed -i -e 's/const,/const /g' -e 's/tolua_outside//g' -e 's/\(Generated automatically by tolua++-[\.0-9]*\) on [A-Za-z0-9: ]*\./\1./' -e 's/tolua++\.h/components\/lua\/tolua++\.h/' $3
