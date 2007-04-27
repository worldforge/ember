#! /bin/sh
tolua++ -n Helpers Helpers.pkg | sed -e 's/const,/const /g' > lua_Helpers.cpp