#! /bin/sh
tolua++ -n EmberOgre EmberOgre.pkg | sed -e 's/const,/const /g' > lua_EmberOgre.cpp