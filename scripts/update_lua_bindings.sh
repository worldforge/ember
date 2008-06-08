#! /bin/sh
#tolua++ will for some reason translate "const std::string" into "const,std::string", so we need to remove these incorrect commas from the final code
#some versions will also for some unexplainable reason not correctly remove the tolua specific directive tolua_outside, so we need to clean that out also
echo "Updating lua bindings."
tolua++ -n $1 $2 | sed -e 's/const,/const /g' | sed -e 's/tolua_outside//g' > $3
