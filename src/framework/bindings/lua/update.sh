#! /bin/sh
tolua++ -n Framework Framework.pkg | sed -e 's/const,/const /g' > lua_Framework.cpp