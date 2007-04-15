#! /bin/sh
tolua++ -n Helpers Helpers | sed -e 's/const,/const /g' > lua_Helpers.cpp