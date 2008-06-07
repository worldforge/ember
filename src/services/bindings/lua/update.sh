#! /bin/sh
tolua++ -n EmberServices EmberServices.pkg | sed -e 's/const,/const /g' > lua_EmberServices.cpp