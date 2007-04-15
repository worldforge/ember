#! /bin/sh
tolua++ -n Atlas Atlas.pkg | sed -e 's/const,/const /g' > lua_Atlas.cpp