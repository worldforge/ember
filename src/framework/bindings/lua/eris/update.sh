#! /bin/sh
tolua++ -n Eris Eris.pkg | sed -e 's/const,/const /g' > lua_Eris.cpp